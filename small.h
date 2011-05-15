#include <vector>
#include <algorithm>

using namespace std;

vector<vector<double> > matrix; //addressing: matrix[row][column]
vector<vector<double> > X;
vector<vector<double> > C;
vector<vector<double> > G; //here stored marks: strokes'n'stars
vector<double> customers, sellers;

//last zero with stroke
pair <int,int> lStroke;

enum {
	No, Stroke, Star
};

#define UINT unsigned int
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

double sum(vector<double> x);
bool comp(int a, int b);
bool isSignificant(int i, int j);

double sum(vector<double> x) {
	double ret = 0;
	for (UINT i = 0; i < x.size(); ++i)
		ret += x[i];
	return ret;
}

inline bool comp(int a, int b) {
	return (a < b);
}

inline bool isSignificant(int i, int j) {
	if (X[i][j] && !C[i][j])
		return true;
	return false;
}

//In russian we call it 'невязка'
class Disrepancy {
public:
	void compute();
	double total;
} disrepancy;

void Disrepancy::compute() {
	total = 0;
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			total += X[i][j];
		}
	}
}

void input() {
	int a, b;
	ifstream in("test");
	in >> a >> b;
	sellers.resize(a), customers.resize(b);
	matrix.resize(a);
	for (int i = 0; i < a; ++i) {
		in >> sellers[i];
		matrix[i].resize(b, 0);
	}
	for (int i = 0; i < b; ++i)
		in >> customers[i];
	for (int i = 0; i < a; ++i)
		for (int j = 0; j < b; j++)
			in >> matrix[i][j];
}

void balance() {
	double c = sum(customers);
	double s = sum(sellers);
	if (s > c) {
		customers.push_back(s - c);
		for (UINT i = 0; i < sellers.size(); ++i) {
			matrix[i].push_back(0); //add column
		}
	} else if (s < c) {
		sellers.push_back(c - s);
		//add row
		matrix[0].push_back(0);
		matrix[customers.size()].resize(customers.size(), 0);
	}
}

void form_C() {
	//create
	C.resize(sellers.size());
	for (UINT j = 0; j < sellers.size(); ++j) {
		C[j].resize(customers.size(), 0);
	}
	//form C-zeroth
	double min = DBL_MAX;
	for (UINT j = 0; j < customers.size(); ++j) {
		for (UINT i = 0; i < sellers.size(); ++i) {
			if (matrix[i][j] < min)
				min = matrix[i][j];
		}
		for (UINT i = 0; i < sellers.size(); ++i) {
			C[i][j] = matrix[i][j] - min;
		}
		min = DBL_MAX;
	}
	//form C'
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			if (C[i][j] < min)
				min = C[i][j];
		}
		for (UINT j = 0; j < customers.size(); ++j) {
			C[i][j] = C[i][j] - min;
		}
		min = DBL_MAX;
	}
}

void form_X() {
	//create
	X.resize(sellers.size());
	for (UINT j = 0; j < sellers.size(); ++j)
		X[j].resize(customers.size(), 0);
	//form and correction
	for (UINT j = 0; j < customers.size(); ++j) {
		for (UINT i = 0; i < sellers.size(); ++i) {
			if (C[i][j] == 0) {
				X[i][j] = MIN(sellers[i], customers[j]);
				sellers[i] -= X[i][j];
				customers[j] -= X[i][j];
			}
		}
	}
}

void preliminary_stage() {
	balance();
	form_C();
	form_X();
}

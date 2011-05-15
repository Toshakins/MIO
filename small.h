#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<vector<int> > matrix; //addressing: matrix[row][column]
vector<vector<int> > X;
vector<vector<int> > C;
vector<vector<int> > G; //here stored marks: strokes'n'stars
vector<int> customers, sellers;

//last zero with stroke
pair <int,int> lStroke;

enum {
	No, Stroke, Star
};

#define UINT unsigned int
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define ORDER 1000

int sum(vector<int> x);
bool comp(int a, int b);
bool isSignificant(int i, int j);

int sum(vector<int> x) {
	int ret = 0;
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
	int total;
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
	double tmp;
	in >> a >> b;
	sellers.resize(a), customers.resize(b);
	matrix.resize(a);
	for (int i = 0; i < a; ++i) {
		in >> tmp;
		sellers[i] = tmp * ORDER;
		matrix[i].resize(b, 0);
	}
	for (int i = 0; i < b; ++i) {
		in >> tmp;
	customers[i] = tmp * ORDER;
	}
	for (int i = 0; i < a; ++i)
		for (int j = 0; j < b; j++){
			in >> tmp;
	matrix[i][j] = tmp * ORDER;}
}

void balance() {
	int c = sum(customers);
	int s = sum(sellers);
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
	int min = INT_MAX;
	for (UINT j = 0; j < customers.size(); ++j) {
		for (UINT i = 0; i < sellers.size(); ++i) {
			if (matrix[i][j] < min)
				min = matrix[i][j];
		}
		for (UINT i = 0; i < sellers.size(); ++i) {
			C[i][j] = matrix[i][j] - min;
		}
		min = INT_MAX;
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
		min = INT_MAX;
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

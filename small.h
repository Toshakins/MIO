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
	vector<double> row, column;
	void compute();
	double total;
	Disrepancy();
} disrepancy;

Disrepancy::Disrepancy() {
	row.resize(sellers.size(), 0);
	column.resize(customers.size(), 0);
}

void Disrepancy::compute() {
	double sum_row = 0, sum_column = 0;
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			sum_column += X[i][j];
		}
		row[i] = sellers[i] - sum_column;
		sum_column = 0;
	}
	for (UINT j = 0; j < customers.size(); ++j) {
		for (UINT i = 0; i < sellers.size(); ++i) {
			sum_row += X[i][j];
		}
		column[j] = customers[j] - sum_row;
		sum_row = 0;
	}
	sum_row = sum_column = 0;
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			sum_row += row[j];
		}
		sum_column += column[i];
	}
	total = sum_row + sum_column;
}

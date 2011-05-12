#include <vector>

using namespace std;

vector<vector<double> > matrix; //addressing: matrix[row][column]
vector<vector<double> > X;
vector<vector<double> > C;
vector<double> customers, sellers;

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
	if (a < b) return true;
	else return false;
}

inline bool isSignificant(int i, int j) {
	if (X[i][j] && !C[i][j])
		return true;
	return false;
}

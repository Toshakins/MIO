#include <vector>

void out(vector <int> x) {
	for (UINT i = 0; i < x.size(); ++i) {
		cout << ' ' << x[i];
	}
	cout << endl;
}

void output(vector<vector<int> > x) {
	for (UINT i = 0; i < sellers.size(); ++i) {
		cout << ' ' << sellers[i];
	}
	cout << endl << endl;
	for (UINT i = 0; i < customers.size(); ++i) {
		cout << ' ' << customers[i];
	}
	cout << endl << endl;
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			cout << ' ' << x[i][j];
		}
		cout << endl;
	}
}

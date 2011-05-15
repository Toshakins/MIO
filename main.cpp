#include <iostream>
#include <cfloat>
#include <fstream>
#include <algorithm>
#include <utility>
#include "iter.h"

using namespace std;

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

void output(vector<vector<double> > x) {
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

int main(int argc, char *argv[]) {
	input();
	vector<double> sellers_source, customers_source;
	sellers_source.assign(sellers.begin(), sellers.end());
	customers_source.assign(customers.begin(), customers.end());
	preliminary_stage();
	disrepancy.total = sum(customers) + sum(sellers); //only once
	disrepancy.row.resize(sellers.size(), 0);
	disrepancy.column.resize(customers.size(), 0);
	for (UINT i = 0; i < customers.size(); ++i) {
		disrepancy.row[i] = customers[i];
	}
	for (UINT i = 0; i < sellers.size(); ++i) {
		disrepancy.column[i] = sellers[i];
	}
	G.resize(C.size());
	for (UINT i = 0; i < C.size(); ++i)
		G[i].resize(C[0].size());
	while ((sum(disrepancy.row) + sum(disrepancy.column)) - disrepancy.total) {
		mark.ing();
		lStroke.first = lStroke.second = -1;
		while (!search_stage()) {
			eqTransformation_stage();
		}
		correction_stage();
		disrepancy.compute();
	}
	//Return value of purpose function and matrix.
	UINT pf = 0;
	ofstream out("outFile");
	for (UINT i = 0; i < sellers_source.size(); ++i) {
		for (UINT j = 0; j < customers_source.size(); ++j) {
			out << X[i][j] << ' ';
			pf += X[i][j] * matrix[i][j];
		}
		out << endl;
	}
	out << endl << pf << endl;
	return 0;
}

//well, it's all about 'for'

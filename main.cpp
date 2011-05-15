#include <iostream>
#include <cfloat>
#include <fstream>
#include <algorithm>
#include "iter.h"

using namespace std;

int main(int argc, char *argv[]) {
	input();
	vector<double> sellers_source, customers_source;
	sellers_source.assign(sellers.begin(), sellers.end());
	customers_source.assign(customers.begin(), customers.end());
	preliminary_stage();
	disrepancy.compute();
	G.resize(C.size());
	for (UINT i = 0; i < C.size(); ++i)
		G[i].resize(C[0].size());
	while ((sum(sellers) + sum(customers))) {
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

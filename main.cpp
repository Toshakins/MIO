#include <iostream>
#include <vector>
#include <cfloat>
#include <fstream>
#include <algorithm>
#include <utility>

#define USI unsigned int
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

using namespace std;

vector<vector<double> > matrix; //addressing: matrix[row][column]
vector<vector<double> > X;
vector<vector<double> > C;
vector<double> customers, sellers;

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

void output(vector <vector <double> > x) {
	for (USI i = 0; i < sellers.size(); ++i) {
		cout << ' ' << sellers[i];
	}
	cout << endl << endl;
	for (USI i = 0; i < customers.size(); ++i) {
		cout << ' ' << customers[i];
	}
	cout << endl << endl;
	for (USI i = 0; i < sellers.size(); ++i) {
		for (USI j = 0; j < customers.size(); ++j) {
			cout << ' ' << x[i][j];
		}
		cout << endl;
	}
}

double sum(vector<double> x) {
	double ret = 0;
	for (USI i = 0; i < x.size(); ++i)
		ret += x[i];
	return ret;
}

void balance() {
	double c = sum(customers);
	double s = sum(sellers);
	if (s > c) {
		customers.push_back(s - c);
		for (USI i = 0; i < sellers.size(); ++i) {
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
	for (USI j = 0; j < sellers.size(); ++j) {
		C[j].resize(customers.size(), 0);
	}
	//form C-zeroth
	double min = DBL_MAX;
	for (USI j = 0; j < customers.size(); ++j) {
		for (USI i = 0; i < sellers.size(); ++i) {
			if (matrix[i][j] < min)
				min = matrix[i][j];
		}
		for (USI i = 0; i < sellers.size(); ++i) {
			C[i][j] = matrix[i][j] - min;
		}
		min = DBL_MAX;
	}
	//form C'
	for (USI i = 0; i < sellers.size(); ++i) {
		for (USI j = 0; j < customers.size(); ++j) {
			if (C[i][j] < min)
				min = C[i][j];
		}
		for (USI j = 0; j < customers.size(); ++j) {
			C[i][j] = C[i][j] - min;
		}
		min = DBL_MAX;
	}
}

void form_X() {
	//create
	X.resize(sellers.size());
	for (USI j = 0; j < sellers.size(); ++j)
		X[j].resize(customers.size(), 0);
	//form and correction
	for (USI j = 0; j < customers.size(); ++j) {
		for (USI i = 0; i < sellers.size(); ++i) {
			if (C[i][j] == 0) {
				X[i][j] = MIN(sellers[i], customers[j]);
				sellers[i] -= X[i][j];
				customers[j] -= X[i][j];
			}
		}
	}
}

//In russian we call it 'невязка'
class Disrepancy {
public:
	vector<double>row, column;
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
	for (USI i = 0; i < sellers.size(); ++i) {
		for (USI j = 0; j < customers.size(); ++j) {
			sum_column += X[i][j];
		}
		row[i] = sellers[i] - sum_column;
		sum_column = 0;
	}
	for (USI j = 0; j < customers.size(); ++j) {
		for (USI i = 0; i < sellers.size(); ++i) {
			sum_row += X[i][j];
		}
		column[j] = customers[j] - sum_row;
		sum_row = 0;
	}
	sum_row = sum_column = 0;
	for (USI i = 0; i < sellers.size(); ++i) {
		for (USI j = 0; j < customers.size(); ++j) {
			sum_row += row[j];
		}
		sum_column += column[i];
	}
	total = sum_row + sum_column;
}

void preliminary_stage() {
	balance();
	form_C();
	form_X();
}

enum markers{Nothing, Plus};

class Mark {
public:
	vector <short int> row, column;
	Mark() {
		row.resize(sellers.size(), Nothing);
		column.resize(customers.size(), Nothing);
	}
	void ing() {
		for (USI i = 0; i < sellers.size(); ++i) {
			if (disrepancy.column[i] == 0)
				column[i] = Plus;
		}
	}
} mark;

vector<pair<int, int> >strokes, stars;

void correction_stage(){
	;
}

bool isSignificant(int i, int j);

inline bool isSignificant(int i, int j) {
	if (X[i][j] && !C[i][j])
		return true;
	return false;
}

void eq_stage() {
	;
}

void search_stage() {
	for (USI j = 0; j < customers.size(); ++j) {
		vector<short int>::iterator it = find(mark.column.begin(), mark.column\
				.end(), Plus);
		if (it == mark.column.end() && *mark.column.end() != Plus)
			continue;
		for (USI i = 0; i < sellers.size(); ++i) {
			if (C[i][j] == 0) {
				strokes.push_back(make_pair(i, j));
				if (disrepancy.row[i] > 0) {
					correction_stage();
					return;
				}
				else {
					mark.row[i] = Plus;
					for (USI k = 0; k < customers.size(); ++k) {
							vector<short int>::iterator itt = find(mark.column.begin(), mark.column\
									.end(), Nothing);
							if (it == mark.column.end() && *mark.column.end() != Nothing)
								continue;
							if (isSignificant(i, k)) {
								mark.column[k] = Nothing;
								stars.push_back(make_pair(i, k));
							}
							//may I forget search in unmarked column...
					}
				}
			}
		}
	}
	eq_stage();
}

int main(int argc, char *argv[]) {
	input();
	vector <double> sellers_source, customers_source;
	sellers_source.assign(sellers.begin(), sellers.end());
	customers_source.assign(customers.begin(), customers.end());
	preliminary_stage();
	disrepancy.total = sum(customers) + sum(sellers); //only once
	for (USI i = 0; i < sellers.size(); ++i) {
		disrepancy.row[i] = sellers[i];
	}
	for (USI i = 0; i < customers.size(); ++i) {
		disrepancy.column[i] = customers[i];
	}
	while (disrepancy.total) {
		mark.ing();
		search_stage();
	}
	//Return value of purpose function and matrix.
	USI pf = 0;
	ofstream out("outFile");
	for (USI i = 0; i < sellers_source.size(); ++i) {
		for (USI j = 0; j < customers_source.size(); ++j) {
			out << X[i][j] << ' ';
			pf += X[i][j] * matrix[i][j];
		}
		out << endl;
	}
	out << endl << pf << endl;
	return 0;
}

//well, it's all about 'for'

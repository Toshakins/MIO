#include <vector>
#include <algorithm>
#include "small.h"

using namespace std;

class Mark {
public:
	//(un)marked columns, rows
	vector<int> markedC, unmarkedC, markedR, unmarkedR;
	void ing() {
		for (UINT i = 0; i < sellers.size(); ++i) {
			for (UINT j = 0; j < customers.size(); ++j) {
				G[i][j] = No;
			}
		}
		markedC.clear(), unmarkedC.clear(), markedR.clear(), unmarkedR.clear();
		for (UINT i = sellers.size(); i >= 0; --i) {
			if (disrepancy.column[i] == 0) {
				markedC.push_back(i);
			} else
				unmarkedC.push_back(i);
		}
		for (UINT i = 0; i < customers.size(); ++i) {
			unmarkedR.push_back(i);
		}
	}
} mark;

bool search_stage() {
	for (UINT i = 0; i < mark.unmarkedC.size(); i++) {
		for (UINT j = 0; j < customers.size(); j++) {
			if (C[j][mark.unmarkedC[i]] == 0 && (G[j][mark.unmarkedC[i]]
					!= Stroke && G[j][mark.unmarkedC[i]] != Star)) {
				G[j][mark.unmarkedC[i]] = Stroke;
				lStroke.first = mark.unmarkedC[i]; //row of begin
				lStroke.second = j; //column of end
				if (disrepancy.row[j] > 0)
					return 1; //call correction of X
				mark.markedR.push_back(j);
				remove(mark.unmarkedR.begin(), mark.unmarkedR.end(), j);
				sort(mark.markedC.begin(), mark.markedC.end(), comp);
				for (UINT k = 0; k < mark.markedC.size(); ++k) {
					if (isSignificant(j, k)) {
						G[j][k] = Star;
						remove(mark.markedC.begin(), mark.markedC.end(), k);
					}
				}
			}
		}
	}
	return 0;
}

void eqTransformation_stage() {
	double h = 0;
	bool found = false;
	sort(mark.unmarkedC.begin(), mark.unmarkedC.end(), comp);
	sort(mark.unmarkedR.begin(), mark.unmarkedR.end(), comp);
	for (UINT i = 0; i < mark.unmarkedR.size(); ++i) {
		for (UINT j = 0; j < mark.unmarkedC.size(); ++j) {
			if (C[i][j] > 0 && C[i][j] > h) {
				h = C[i][j];
				found = true;
			}
		}
	}
	if (!found) {
		cout << "Can't transform C" << endl;
		exit(0);
	}
	//add to marked columns and subtract from unmarked rows
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < mark.markedC.size(); ++j) {
			C[i][j] += h;
		}
	}
	for (UINT i = 0; i < mark.unmarkedR.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			C[i][j] -= h;
		}
	}
}

int getStroke(UINT row) {
	for (UINT i = 0; i < customers.size(); ++i) {
		if (G[row][i] == Stroke)
			return i;
	}
	return -1;
}

int getStar(UINT column) {
	for (UINT i = 0; i < sellers.size(); ++i) {
		if (G[i][column] == Star)
			return i;
	}
	return -1;
}

struct container {
	int x, y;
	double val;
};

bool containerComp(container a, container b);
inline bool containerComp(container a, container b) {
	return (a.val < b.val);
}

void correction_stage() {
	//circuit maker ^_^
	int row_of_star, col_of_stroke, col_of_star = lStroke.first;
	vector<container> stars, strokes;
	container tmp;
	tmp.x = lStroke.first, tmp.y = lStroke.second, tmp.val
			= X[lStroke.first][lStroke.second];
	strokes.push_back(tmp);
	while ((row_of_star = getStar(col_of_star)) >= 0) {
		if ((col_of_stroke = getStroke(row_of_star)) >= 0) {
			tmp.x = row_of_star, tmp.y = col_of_star, tmp.val = X[tmp.x][tmp.y];
			stars.push_back(tmp);
			tmp.x = row_of_star, tmp.y = col_of_stroke, tmp.val
					= X[tmp.x][tmp.y];
			strokes.push_back(tmp);
			lStroke.second = col_of_stroke;//column of end
			col_of_star = col_of_stroke;
		}
	}
	//prepare for combo!
	double teta = min(
			min_element(stars.begin(), stars.end(), containerComp)->val,
			min(disrepancy.row[lStroke.first],
					disrepancy.column[lStroke.second]));
	for (UINT i = 0; i < strokes.size(); ++i) {
		X[strokes[i].x][strokes[i].y] += teta;
	}
	for (UINT i = 0; i < stars.size(); ++i) {
			X[stars[i].x][stars[i].y] -= teta;
		}
}

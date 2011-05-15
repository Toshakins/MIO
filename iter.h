#include <vector>
#include <algorithm>
#include "small.h"
#include "debug.h"
#include <cstdlib>
#include <climits>

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
		for (UINT i = 0; i < customers.size(); ++i) {
			if (customers[i] == 0) {
				markedC.push_back(i);
			} else
				unmarkedC.push_back(i);
		}
		for (UINT i = 0; i < sellers.size(); ++i) {
			unmarkedR.push_back(i);
		}
	}
} mark;

bool search_stage() {
	l:for (UINT i = 0; i < mark.unmarkedC.size(); i++) {
		for (UINT j = 0; j < sellers.size(); j++) {
			if ((C[j][mark.unmarkedC[i]] == 0) && ((G[j][mark.unmarkedC[i]]
					!= Stroke) && (G[j][mark.unmarkedC[i]] != Star))) {
				G[j][mark.unmarkedC[i]] = Stroke;
				lStroke.first = j; //row of begin
				lStroke.second = mark.unmarkedC[i]; //column of end
				if (sellers[j] > 0) {
					return 1; //call correction of X
				}
				mark.markedR.push_back(j);
				remove(mark.unmarkedR.begin(), mark.unmarkedR.end(), j);
				mark.unmarkedR.resize(mark.unmarkedR.size() - 1);
				sort(mark.markedC.begin(), mark.markedC.end(), comp);
				for (UINT k = 0; k < mark.markedC.size(); ++k) {
					if (isSignificant(j, mark.markedC[k]) && (G[j][mark.markedC[k]] != Star)) {
						G[j][mark.markedC[k]] = Star;
						mark.unmarkedC.push_back(mark.markedC[k]);
						sort(mark.unmarkedC.begin(), mark.unmarkedC.end(), comp);
						remove(mark.markedC.begin(), mark.markedC.end(),
								mark.markedC[k]);
						mark.markedC.resize(mark.markedC.size() - 1);
						goto l;
					}
				}
			}
		}
	}
	return 0;
}

void eqTransformation_stage() {
	double h = INT_MAX;
	bool found = false;
	sort(mark.unmarkedC.begin(), mark.unmarkedC.end(), comp);
	sort(mark.unmarkedR.begin(), mark.unmarkedR.end(), comp);
	for (UINT i = 0; i < mark.unmarkedR.size(); ++i) {
		for (UINT j = 0; j < mark.unmarkedC.size(); ++j) {
			if ((C[mark.unmarkedR[i]][mark.unmarkedC[j]] > 0)
					&& (C[mark.unmarkedR[i]][mark.unmarkedC[j]] < h)) {
				h = C[mark.unmarkedR[i]][mark.unmarkedC[j]];
				found = true;
			}
		}
	}
	if (!found) {
		system("echo 'Can not transform C'");
		exit(1);
	}
	//add to marked columns and subtract from unmarked rows
	for (UINT i = 0; i < sellers.size(); ++i) {
		for (UINT j = 0; j < mark.markedC.size(); ++j) {
			C[i][mark.markedC[j]] += h;
		}
	}
	for (UINT i = 0; i < mark.unmarkedR.size(); ++i) {
		for (UINT j = 0; j < customers.size(); ++j) {
			C[mark.unmarkedR[i]][j] -= h;
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
	int row_of_star, col_of_stroke, col_of_star = lStroke.second;
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
	double teta;
	if (strokes.size() != 1) {
		//prepare for combo!
		teta = min(
				min_element(stars.begin(), stars.end(), containerComp)->val,
				min(sellers[lStroke.first],
						customers[lStroke.second]));
	} else {
		teta = min(sellers[lStroke.first],
				customers[lStroke.second]);
	}
	for (UINT i = 0; i < strokes.size(); ++i) {
		X[strokes[i].x][strokes[i].y] += teta;
		sellers[strokes[i].x] -= teta;
		customers[strokes[i].y] -= teta;
	}
	for (UINT i = 0; i < stars.size(); ++i) {
		X[stars[i].x][stars[i].y] -= teta;
		sellers[stars[i].x] += teta;
		customers[stars[i].y] += teta;
	}
}

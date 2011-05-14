#include <vector>
#include "small.h"

using namespace std;

class Mark {
public:
	//(un)marked columns, rows
	vector <int> markedC, unmarkedC, markedR, unmarkedR;
	vector<vector<int> > mmark;
	Mark() {
		//mmark.resize()
	}
	void ing() {
		for (UINT i = sellers.size(); i >= 0; --i) {
			if (disrepancy.column[i] == 0) {
				markedC.push_back(i);
			}
			else unmarkedC.push_back(i);
		}
		for (UINT i = 0; i < customers.size(); ++i){
			unmarkedR.push_back(i);
		}
	}
} mark;

bool search_stage() {
	for (UINT i = 0; i < mark.unmarkedC.size(); i++) {
		for (UINT j = 0; j < customers.size(); j++) {
			if (C[j][mark.unmarkedC[i]] == 0){
				if (disrepancy.row[j] > 0) return 1; //call correction of X
				mark.markedR.push_back(j);
				remove(mark.unmarkedR.begin(), mark.unmarkedR.end(), j);
			}
		}
	}
	return 0;
}

void correction_stage(){
	//circuit maker ^_^
	;
}

void eqTransformation_stage() {

}

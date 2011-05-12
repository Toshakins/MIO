#include <vector>
#include "small.h"

using namespace std;

class Mark {
public:
	//(un)marked columns, rows
	vector <int> markedC, unmarkedC, markedR, unmarkedR;
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

	}
	return 0;
}

void correction_stage(){
	//circuit maker ^_^
	;
}

void eqTransformation_stage() {

}

#include "motzkin.h"

void printGrowthConstant(int maximumIteration, Motzkin& motzkinInfo){
	IterationVector old_vector(motzkinInfo);
	for (int i=2; i<=maximumIteration; i++) {
		IterationVector new_vector = old_vector.iterate();
		cout << "n="<<i<<": "<<new_vector.cell_value(1)<<endl;
		// compute, print
		old_vector = new_vector;
	}
}

int main(){
	const int W=5;
	Motzkin test(W+1);
	printGrowthConstant(50,test);
// 	IterationVector testVector(test.motzkinPathsNumber());
// 	testVector.printSpecificCell(1,50,test);

	return 0;
}

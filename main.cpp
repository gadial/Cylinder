#include "motzkin.h"

int main(){
	const int W=5;
// 	VectorKnownValueMap knownValues;
// 	readKnownValues(knownValues,"knownVectorValues.txt");
//	for (int W=1; W<20; W++){
	Motzkin test(W+1);
	IterationVector testVector(test.motzkinPathsNumber());
	testVector.printSpecificCell(1,50,test);
//	testVector.init(1);
//	testVector.print();

//	cout << test.motzkinPathsNumber()<<endl;
//	cout << computeVectorCell(15,20,test)<<endl;
//	test.print();
/*
	for (mpz_class i=1; i<test.motzkinPathsNumber(); i++){
		cout.width(2);
		cout << i<<": ";
//		test.printSets(i);
		cout << " ["<<test.succ0(i)<<","<<test.succ1(i)<<"]"<<endl;
	}
*/
/*
	int count=0;
	for (int i=1; i<test.motzkinPathsNumber(); i++){
		if (test.succ0(i)==-1) count++;//cout <<i<<endl;
	}
	cout << W<<": "<<count<<endl;
}
*/
/*
	int* succ0Usage=new int[test.motzkinPathsNumber()];
	int* succ1Usage=new int[test.motzkinPathsNumber()];
	for (int i=1; i<test.motzkinPathsNumber(); i++){
		int temp=test.succ0(i);
		if (temp!=-1) 
			succ0Usage[temp-1]++;
			succ1Usage[test.succ1(i)-1]++;
	}
	cout << "succ0 usage:"<<endl;
	cout << "[";
	for (int i=0; i<test.motzkinPathsNumber(); i++)
		cout << succ0Usage[i]<<" ";
	cout << "]"<<endl;

	cout << "succ1 usage:"<<endl;
	cout << "[";
	for (int i=0; i<test.motzkinPathsNumber(); i++)
		cout << succ1Usage[i]<<" ";
	cout << "]"<<endl;


	MotzkinPath temp=test.numberToMotzkinPath(241);

	temp.set(tempPath);
	temp.print();
	temp.printTrail();
	cout << "A="<<temp.findA()<<" B="<<temp.findB()<<" C="<<temp.findC()<<endl;
	
	Set< Set<int> > tempSet=temp.convertgitkToSetOfSets();
	cout << tempSet << endl;
	MotzkinPath tempSucc=temp.succ1();
	tempSet=tempSucc.convertToSetOfSets();
	cout << tempSet << endl;
	tempSucc.print();
	tempSucc.printTrail();
*/
	return 0;
}

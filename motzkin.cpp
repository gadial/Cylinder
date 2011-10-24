#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "bigint.h"
using namespace std;
class ErrorType{
};

class ErrorNotMotzkinPathNumber:public ErrorType{};
class MotzkinPathLengthDoNotMatch:public ErrorType{};
class ACouldNotCompute:public ErrorType{};
class BCouldNotCompute:public ErrorType{};
class CCouldNotCompute:public ErrorType{};
class succ0ReturnNull:public ErrorType{};
class succ0Fail:public ErrorType{};
class succ1Fail:public ErrorType{};

//typedef int BigInt;
typedef pair<int,BigInt> KnownValuesMapCoordinates; //first index is iteration number, second is place in vector
typedef map<KnownValuesMapCoordinates,BigInt> VectorKnownValueMap; 
// a Motzkin path (of length n) is a path from (0,0) to (0,n) which is always above the x-axis
// and contains exactly n steps. Each step is "go right" which can be combined with "go up" or
// "go down". Henceforth, we represent it as a size n array of 1 ("up"), 0 ("stay") and -1 ("down")
template<class T>
class Set{
	public:
	Set():size(0),items(){};
	void add(T item){items.push_back(item); size++;}
	int size;
	vector<T> items;
};

template<class T>
ostream& operator<<(ostream& o, Set<T> rhs){
	o<<"{";
	for (int i=0; i<rhs.size; i++){
		if (i>0) o<<", ";
		o<<rhs.items[i];
	}
	o<<"}";
	return o;
}

class MotzkinPath{
	public:
	MotzkinPath(int n):length(n),steps(new int[n]){}
	MotzkinPath(const MotzkinPath& old):length(old.length),steps(new int[old.length]){
		for (int i=0; i<length; i++) steps[i]=old.steps[i];
	}
	MotzkinPath& operator=(const MotzkinPath& old){
				if (this==&old) return *this;
				delete[] steps;
				length=old.length; steps=new int[length];
				for (int i=0; i<length; i++) steps[i]=old.steps[i];
				return *this;
	}
	~MotzkinPath(){delete[] steps;}
	void print(){cout << "("; for (int i=0; i<length; i++) {cout.width(3); cout << steps[i];} cout<<")"<<endl;}
	void printTrail();
	void printSets();
	void shiftRight(int offset=1);
	int findA();
	int findB();
	int findC();
	Set< Set<int> > convertToSetOfSets();
	MotzkinPath succ0();
	MotzkinPath succ1();
	void set(int* newSteps){for (int i=0; i<length; i++) steps[i]=newSteps[i];}
	int length;
	int* steps;
};

void MotzkinPath::printSets(){
	cout << convertToSetOfSets();
}
// page 21 in the article
Set< Set<int> > MotzkinPath::convertToSetOfSets(){
	int W=length-1;
	int level=0;
	Set< Set<int> > signature;
	Set<int>** currentLevel=new Set<int>*[length];
	for (int i=0; i<=W; i++){
		if (steps[i]==1){
			level++;
			if (level%2==1)
				currentLevel[level]=new Set<int>;
		}
		else if (steps[i]==-1){
			if (level%2==1){
				signature.add(*currentLevel[level]);
				delete currentLevel[level];
			}
			level--;
		}
		if (level%2==1)
			currentLevel[level]->add(i+1);				
	}
	delete[] currentLevel;
	return signature;
}

void MotzkinPath::shiftRight(int offset){
	for (int i=length-1; i>=0; i--){
		if (i-offset>=0)
			steps[i]=steps[i-offset];
		else
			steps[i]=0;
	}
}

void MotzkinPath::printTrail() {
	cout << "(";
	int i=0; 
	for (int j=0; j<length; j++) {
		cout.width(3); 
		cout <<i;
		i+=steps[j]; 
	} 
	cout.width(3); 
	cout <<i<<")"<<endl;
}
//A is defined as the smallest index>0 for which the path is of height 0
int MotzkinPath::findA(){
	int i=0; //current height
	for (int j=0; j<length; j++){
		i+=steps[j];
		if (i==0) return (j+1);
	}
	throw ACouldNotCompute();
}
//B is defined as the largest index<length for which the path is of height 0
int MotzkinPath::findB(){
	int i=0; //current height
	for (int j=length-1; j>=0; j--){
		i-=steps[j]; // going "backwards"
		if (i==0) return (j);
	}
	throw BCouldNotCompute();
}
//C is defined as the largest index<length-1 for which the path is of height 1
int MotzkinPath::findC(){
	int i=0; //current height
	for (int j=length-1; j>=0; j--){
		i-=steps[j]; // going "backwards"
		if (i==1 && j<length-1) return (j);
	}
	throw CCouldNotCompute();
}

// based on pg. 24 in the article
MotzkinPath MotzkinPath::succ0(){
	MotzkinPath temp=*this;
	int W=length-1; // to be coherent with the article
	int B,C;
	switch (temp.steps[W]){
		case 0: // return (0,p[0..W-1])
			temp.shiftRight();
			return temp;
			break;
		case -1:
			switch(temp.steps[W-1]){
				case 0: // return (0,p[0..W-2],-1)
					temp.shiftRight();
					temp.steps[W]=-1;
					return temp;
					break;
				case -1: // return (0,p[0..B-1],-1,p[B+1..W-2],0)
					C=temp.findC();
					temp.steps[C]=-1;
					temp.shiftRight();
					temp.steps[W]=0;
					return temp;
					break;
				case 1: // return null
					throw succ0ReturnNull();
			}
			break;
	}
	throw succ0Fail();
}

// based on pg. 25 in the article
MotzkinPath MotzkinPath::succ1(){
	MotzkinPath temp=*this;
	int W=length-1; // to be coherent with the article
	int A,B,C;
	//for simplicity, first we deal with one case, then the others
	if (temp.steps[W]==0){
		switch (temp.steps[0]){
			case 0: // return (1,-1,p[1..W-1])
				temp.shiftRight();
				temp.steps[0]=1;
				temp.steps[1]=-1;
				return temp;
				break;
			case 1: // return (1,0,p[1..W-1])
				temp.shiftRight();
				temp.steps[0]=1;
				temp.steps[1]=0;
				return temp;
				break;
		}	
		throw succ1Fail();
	}
	if (temp.steps[W]==1) throw succ1Fail();
	//now we can assume steps[W]==-1;
	switch (temp.steps[W-1]){
		case 1:
			switch (temp.steps[0]){
				case 0: // return (1,-1,p[1..W-2],0)
					temp.shiftRight();
					temp.steps[0]=1;
					temp.steps[1]=-1;
					temp.steps[W]=0;
					return temp;
					break;
				case 1: // return (1,0,p[1..W-2],0)
					temp.shiftRight();
					temp.steps[0]=1;
					temp.steps[1]=0;
					temp.steps[W]=0;
					return temp;
					break;
			}
			break;
		case 0:
			switch (temp.steps[0]){
				case 0: // return (1,1,p[1..B-1],-1,p[B+1..W-2],-1)
					B=temp.findB();
					temp.steps[B]=-1;
					temp.shiftRight();
					temp.steps[0]=1;
					temp.steps[1]=1;
					temp.steps[W]=-1;
					return temp;
					break;
				case 1: 
					A=temp.findA();
					if (A==W+1){ // return [1,0,p[1..W-2],-1)
						temp.shiftRight();
						temp.steps[0]=1;
						temp.steps[1]=0;
						temp.steps[W]=-1;
						return temp;
					}
					else{ //return (1,0,p[1..A-2],1,p[A..B-1],-1,p[B+1..W-2],-1)
						B=temp.findB();
						temp.steps[A-1]=1;
						temp.steps[B]=-1;
						temp.shiftRight();
						temp.steps[0]=1;
						temp.steps[1]=0;
						temp.steps[W]=-1;
						return temp;
					}
					break;
			}
			break;
		case -1:
			switch (temp.steps[0]){
				case 0: // return (1,1,p[1..B-1],-1,p[B+1..C-1],-1,p[C+1..W-2],0)
					B=temp.findB();
					C=temp.findC();
					temp.steps[B]=-1;
					temp.steps[C]=-1;
					temp.shiftRight();
					temp.steps[0]=1;
					temp.steps[1]=1;
					temp.steps[W]=0;
					return temp;
					break;
				case 1: 
					A=temp.findA();
					if (A==W+1){ // return (1,0,p[1..C-1],-1,p[C+1..W-2],0)
						C=temp.findC();
						temp.steps[C]=-1;
						temp.shiftRight();
						temp.steps[0]=1;
						temp.steps[1]=0;
						temp.steps[W]=0;
						return temp;						
					}
					else{ //return (1,0,p[1..A-2],1,p[A..B-1],-1,p[B+1..C-1],-1,p[C+1..W-2],0)
						B=temp.findB();
						C=temp.findC();
						temp.steps[A-1]=1;
						temp.steps[B]=-1;
						temp.steps[C]=-1;
						temp.shiftRight();
						temp.steps[0]=1;
						temp.steps[1]=0;
						temp.steps[W]=0;
						return temp;
					}
					break;	
			}
			break;
	}
	throw succ1Fail();
}

// Motzkin contains handles the conversions between numbers and motzkin paths
// It is initialized with the length of the moztkin path, and than is able to convert
// numbers to specific paths and vice versa

class Motzkin{
	public:
	Motzkin(int n);
	BigInt sumNeighbors(int i, int j);
	MotzkinPath numberToMotzkinPath(BigInt number);
	BigInt motzkinPathToNumber(MotzkinPath path);
	void print(int width=5);
	BigInt succ0(BigInt pathNum);
	BigInt succ1(BigInt pathNum);
	BigInt motzkinPathsNumber();
	void printSets(BigInt pathNum);
	int maxHeight;
	int length;
	BigInt** pathsNumber;
};
// returns -1 if succ0 does not exist
void Motzkin::printSets(BigInt pathNum){
	numberToMotzkinPath(pathNum).printSets();
}
BigInt Motzkin::motzkinPathsNumber(){
	return pathsNumber[0][0];
}
BigInt Motzkin::succ0(BigInt pathNum){
	MotzkinPath temp=numberToMotzkinPath(pathNum);
	MotzkinPath succ(length);
	try{
		succ=temp.succ0();
	}
	catch(succ0ReturnNull){
		return -1;
	}
	return motzkinPathToNumber(succ);
}

BigInt Motzkin::succ1(BigInt pathNum){
	MotzkinPath temp=numberToMotzkinPath(pathNum);
	MotzkinPath succ=temp.succ1();
	return motzkinPathToNumber(succ);
}

BigInt Motzkin::sumNeighbors(int i, int j){
	/* for each cell, sum up the three right neighbors (if exist) */
	BigInt sum=0;	

	if (j>=length) return 0; // no right neighbors
	if (i>0) sum+=pathsNumber[i-1][j+1]; // a lower neighbor exists
	if (i<maxHeight) sum+=pathsNumber[i+1][j+1]; // an upper neighbor exists
	sum+=pathsNumber[i][j+1]; // frontal neighbor always exists if j<length
	return sum;	
}

Motzkin::Motzkin(int n):length(n), maxHeight(n/2){
	int i,j;

	/* initalizing the array */
	pathsNumber=new BigInt*[maxHeight+1];
	for (i=0; i<=maxHeight; i++){
		pathsNumber[i]=new BigInt[n+1];
		for (j=0; j<=n; j++)
			pathsNumber[i][j]=0; //initialize everything to 0;
	}
	/* initial condition: 1 for the the rightmost down corner */
	pathsNumber[0][n]=1; 
	/* now continue by induction */
	for (j=n-1; j>=0; j--)
		for (i=0; i<=maxHeight; i++)
			pathsNumber[i][j]=sumNeighbors(i,j);
}

void Motzkin::print(int width){
	int i,j;
	for (i=maxHeight; i>=0; i--){
		for (j=0; j<=length; j++){
			cout.width(width);
			cout << pathsNumber[i][j]<<" ";
		}
		cout << endl;
	}
			
}
MotzkinPath Motzkin::numberToMotzkinPath(BigInt number){
	MotzkinPath temp(length);
	int i=0; // initial height
	if (number>=pathsNumber[0][0]) throw ErrorNotMotzkinPathNumber();

	for (int j=0; j<length; j++){
		if (i>0) { // it is possible to go down
			if (number<pathsNumber[i-1][j+1]) {// not big, should go down
				temp.steps[j]=-1;
				i--;
				continue;
			}
			else
				number-=pathsNumber[i-1][j+1];
		}
		if (number<pathsNumber[i][j+1]) {
			temp.steps[j]=0;
			continue;
		}
		else
			number-=pathsNumber[i][j+1];
		temp.steps[j]=1;
		i++;
	}
	return temp;
}

BigInt Motzkin::motzkinPathToNumber(MotzkinPath path){
	BigInt num=0;
	int i=0;
	if (path.length!=length) throw MotzkinPathLengthDoNotMatch();
	for (int j=0; j<length; j++){
		switch (path.steps[j]){
			case 1:
				num+=pathsNumber[i][j+1];
				if (i>0) num+=pathsNumber[i-1][j+1];
				i++;
				break;
			case 0:
				if (i>0) num+=pathsNumber[i-1][j+1];
				break;
			case -1:
				i--;
				break;
		}
	}
	return num;
}

BigInt defaultVector(BigInt cellNum){
	return 1;
}

BigInt computeVectorCell(int n, BigInt cellNum, Motzkin& motzkinInfo){ // n- iteration number
	if (n==1) return defaultVector(cellNum);
	
	BigInt returnValue;
	returnValue=computeVectorCell(n-1,motzkinInfo.succ1(cellNum),motzkinInfo);
	if (motzkinInfo.succ0(cellNum)!=-1)
		returnValue+=computeVectorCell(n,motzkinInfo.succ0(cellNum),motzkinInfo);
	return returnValue;
}

bool vectorCellValueWorthSaving(int n, BigInt cellNum){
	
	return false;
}

BigInt computeVectorCellExtended(int n, BigInt cellNum, Motzkin& motzkinInfo, VectorKnownValueMap& knownValues){ // n- iteration number
	if (n==1) return defaultVector(cellNum);
	
	VectorKnownValueMap::iterator iter;
	iter=knownValues.find(KnownValuesMapCoordinates(n,cellNum));
	if (iter != knownValues.end())
		return iter->second;
		
	BigInt returnValue;
	returnValue=computeVectorCellExtended(n-1,motzkinInfo.succ1(cellNum),motzkinInfo,knownValues);
	if (motzkinInfo.succ0(cellNum)!=-1)
		returnValue+=computeVectorCellExtended(n,motzkinInfo.succ0(cellNum),motzkinInfo,knownValues);
	if (vectorCellValueWorthSaving(n,cellNum))
		knownValues[KnownValuesMapCoordinates(n,cellNum)]=returnValue;
	return returnValue;
}

class IterationVector{
	typedef BigInt VectorValueType;
	public:
		IterationVector(BigInt _size):size(_size){};
		void init(VectorValueType initValue=1);
		void iterate(Motzkin& motzkinInfo);
		void print();
		void printSpecificCell(BigInt cellToPrint, int maximumIteration, Motzkin& motzkinInfo);
	private:
		BigInt size;
		map<BigInt, VectorValueType> cell;
		
};

void IterationVector::init(VectorValueType initValue){
	for (BigInt i=0; i<size; i++)
		cell[i]=initValue;
}

void IterationVector::iterate(Motzkin& motzkinInfo){
	IterationVector temp(this->size);
	temp.init(0);
	for (BigInt i=0; i<size; i++){
		if (motzkinInfo.succ0(i)!=-1)
			temp.cell[i]+=temp.cell[motzkinInfo.succ0(i)];
		temp.cell[i]+=cell[motzkinInfo.succ1(i)];
	}
	*this=temp;
}
void IterationVector::printSpecificCell(BigInt cellToPrint, int maximumIteration, Motzkin& motzkinInfo){
	cout << "Cell number "<<cellToPrint<<endl;
	init(1);
	for (int i=2; i<=maximumIteration; i++) {
		iterate(motzkinInfo);
		cout << "n="<<i<<": "<<cell[cellToPrint]<<endl;
	}
}

void IterationVector::printGrowthConstant(BigInt cellToPrint, int maximumIteration, Motzkin& motzkinInfo){
	cout << "Cell number "<<cellToPrint<<endl;
	init(1);
	for (int i=2; i<=maximumIteration; i++) {
		iterate(motzkinInfo);
		cout << "n="<<i<<": "<<cell[cellToPrint]<<endl;
	}
}
/*
void IterationVector::print(){
	for (BigInt i=0; i<size; i+=4){
		for (int j=0; j<4 && j+i<size; j++){
			cout.width(4);
			cout << i+j<<":";
			cout.width(6);
			cout<<cell[i+j]<<" ";
		}
		cout << endl;			
	}
}
*/
void IterationVector::print(){
	for (BigInt i=0; i<size; i+=1){
			cout << i<<":"<<cell[i]<<" "<<endl;
	}
}

// void readKnownValues(VectorKnownValueMap& knownValues, char* filename){
// 	fstream file(filename);
// 	string a,b;
// 	BigInt 
// }

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
	for (BigInt i=1; i<test.motzkinPathsNumber(); i++){
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
	
	Set< Set<int> > tempSet=temp.convertToSetOfSets();
	cout << tempSet << endl;
	MotzkinPath tempSucc=temp.succ1();
	tempSet=tempSucc.convertToSetOfSets();
	cout << tempSet << endl;
	tempSucc.print();
	tempSucc.printTrail();
*/
	return 0;
}

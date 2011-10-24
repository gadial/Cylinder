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
	int length;
	int maxHeight;
	BigInt** pathsNumber;
};

class IterationVector{
	typedef BigInt VectorValueType;
	public:
		IterationVector(BigInt _size):size(_size){};
		void init(VectorValueType initValue=1);
		void iterate(Motzkin& motzkinInfo);
		void print();
		void printSpecificCell(BigInt cellToPrint, int maximumIteration, Motzkin& motzkinInfo);
		void printGrowthConstant(BigInt cellToPrint, int maximumIteration, Motzkin& motzkinInfo);
	private:
		BigInt size;
		map<BigInt, VectorValueType> cell;
		
};
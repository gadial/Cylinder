#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <gmpxx.h> //The GNU Multiple Precision Arithmetic Library; see http://gmplib.org/

// #include "bigint.h"
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

typedef pair<int,mpz_class> KnownValuesMapCoordinates; //first index is iteration number, second is place in vector
typedef map<KnownValuesMapCoordinates,mpz_class> VectorKnownValueMap; 
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
	mpz_class sumNeighbors(int i, int j);
	MotzkinPath numberToMotzkinPath(mpz_class number);
	mpz_class motzkinPathToNumber(MotzkinPath path);
	void print(int width=5);
	mpz_class succ0(mpz_class pathNum);
	mpz_class succ1(mpz_class pathNum);
	mpz_class motzkinPathsNumber();
	void printSets(mpz_class pathNum);
	int length;
	int maxHeight;
	mpz_class** pathsNumber;
};

class IterationVector{
	typedef mpz_class VectorValueType;
	public:
		IterationVector(mpz_class _size):size(_size){};
		void init(VectorValueType initValue=1);
		void iterate(Motzkin& motzkinInfo);
		void print();
		void printSpecificCell(mpz_class cellToPrint, int maximumIteration, Motzkin& motzkinInfo);
		void printGrowthConstant(mpz_class cellToPrint, int maximumIteration, Motzkin& motzkinInfo);
	private:
		mpz_class size;
		map<mpz_class, VectorValueType> cell;
		
};
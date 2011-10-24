#include "motzkin.h"
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
	int C;
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

// Motzkin handles the conversions between numbers and motzkin paths
// It is initialized with the length of the moztkin path, and than is able to convert
// numbers to specific paths and vice versa


// returns -1 if succ0 does not exist
Motzkin::Motzkin(int n):length(n), maxHeight(n/2){
	int i,j;

	/* initalizing the array */
	pathsNumber=new mpz_class*[maxHeight+1];
	for (i=0; i<=maxHeight; i++){
		pathsNumber[i]=new mpz_class[n+1];
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

Motzkin::Motzkin(const Motzkin& rhs):length(rhs.length), maxHeight(rhs.maxHeight){
	int i,j;
	pathsNumber=new mpz_class*[maxHeight+1];
	for (i=0; i<=maxHeight; i++){
		pathsNumber[i]=new mpz_class[length+1];
		for (j=0; j<=length; j++)
			pathsNumber[i][j]=rhs.pathsNumber[i][j]; 
	}
}

void Motzkin::printSets(mpz_class pathNum){
	numberToMotzkinPath(pathNum).printSets();
}
mpz_class Motzkin::motzkinPathsNumber(){
	return pathsNumber[0][0];
}
mpz_class Motzkin::succ0(mpz_class pathNum){
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

mpz_class Motzkin::succ1(mpz_class pathNum){
	MotzkinPath temp=numberToMotzkinPath(pathNum);
	MotzkinPath succ=temp.succ1();
	return motzkinPathToNumber(succ);
}

mpz_class Motzkin::sumNeighbors(int i, int j){
	/* for each cell, sum up the three right neighbors (if exist) */
	mpz_class sum=0;	

	if (j>=length) return 0; // no right neighbors
	if (i>0) sum+=pathsNumber[i-1][j+1]; // a lower neighbor exists
	if (i<maxHeight) sum+=pathsNumber[i+1][j+1]; // an upper neighbor exists
	sum+=pathsNumber[i][j+1]; // frontal neighbor always exists if j<length
	return sum;	
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
MotzkinPath Motzkin::numberToMotzkinPath(mpz_class number){
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

mpz_class Motzkin::motzkinPathToNumber(MotzkinPath path){
	mpz_class num=0;
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

mpz_class defaultVector(mpz_class cellNum){
	return 1;
}

mpz_class computeVectorCell(int n, mpz_class cellNum, Motzkin& motzkinInfo){ // n- iteration number
	if (n==1) return defaultVector(cellNum);
	
	mpz_class returnValue;
	returnValue=computeVectorCell(n-1,motzkinInfo.succ1(cellNum),motzkinInfo);
	if (motzkinInfo.succ0(cellNum)!=-1)
		returnValue+=computeVectorCell(n,motzkinInfo.succ0(cellNum),motzkinInfo);
	return returnValue;
}

bool vectorCellValueWorthSaving(int n, mpz_class cellNum){
	
	return false;
}

mpz_class computeVectorCellExtended(int n, mpz_class cellNum, Motzkin& motzkinInfo, VectorKnownValueMap& knownValues){ // n- iteration number
	if (n==1) return defaultVector(cellNum);
	
	VectorKnownValueMap::iterator iter;
	iter=knownValues.find(KnownValuesMapCoordinates(n,cellNum));
	if (iter != knownValues.end())
		return iter->second;
		
	mpz_class returnValue;
	returnValue=computeVectorCellExtended(n-1,motzkinInfo.succ1(cellNum),motzkinInfo,knownValues);
	if (motzkinInfo.succ0(cellNum)!=-1)
		returnValue+=computeVectorCellExtended(n,motzkinInfo.succ0(cellNum),motzkinInfo,knownValues);
	if (vectorCellValueWorthSaving(n,cellNum))
		knownValues[KnownValuesMapCoordinates(n,cellNum)]=returnValue;
	return returnValue;
}

void IterationVector::init(VectorValueType initValue, int precision){
	for (mpz_class i=0; i<size; i++){
		cell[i]=initValue;
		if (precision > 0)
			cell[i].set_prec(precision);
	}
}

IterationVector& IterationVector::operator=(const IterationVector& rhs){
	size = rhs.size;
	cell = rhs.cell;
	motzkinInfo = rhs.motzkinInfo;
	return *this;
}

IterationVector IterationVector::iterate(){
	IterationVector temp(motzkinInfo);
	temp.init(0);
	for (mpz_class i=0; i<size; i++){
		if (motzkinInfo.succ0(i)!=-1)
			temp.cell[i]+=temp.cell[motzkinInfo.succ0(i)];
		temp.cell[i]+=cell[motzkinInfo.succ1(i)];
	}
	return temp;
}


#include <iomanip>
#include <stdlib.h>
#include "motzkin.h"

void printGrowthConstant(Motzkin& motzkinInfo, int precision = 15, int maximumIteration = -1){
	cout << setprecision(precision);
	
	IterationVector old_vector(motzkinInfo, precision);
	for (int i=2; (maximumIteration < 0) || i<=maximumIteration; i++) {
		IterationVector new_vector = old_vector.iterate();
		mpf_class temp(new_vector.cell_value(1) / old_vector.cell_value(1), precision);
 		mpf_class min = temp;
		mpf_class max = temp;
		for (mpz_class j=2; j< new_vector.get_size(); j++){
			temp = new_vector.cell_value(j) / old_vector.cell_value(j);
			if (min > temp){
				min = temp;
			}
			if (max < temp){
				max = temp;
			}
		}
		cout << "n="<<i<<": min = "<< min <<", max = " << max <<endl;
		if (mpf_eq(min.get_mpf_t(), max.get_mpf_t(), temp.get_prec()))
			break;
		old_vector = new_vector;
	}
}

int main(int argc, char* argv[]){
	if (argc < 2){
		exit(1);
	}
	int W = atoi(argv[1]);
	int precision = 15;
	int maximumIteration = -1;
	
	if (argc >= 3){
		precision = atoi(argv[2]);
	}
	
	if (argc >= 4){
		maximumIteration = atoi(argv[3]);
	}
	

	Motzkin motzkinInfo(W+1);
	printGrowthConstant(motzkinInfo,precision,maximumIteration);
	return 0;
}

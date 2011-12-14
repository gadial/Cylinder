#include <iomanip>
#include <stdlib.h>
#include "motzkin.h"

mpf_class printGrowthConstant(Motzkin& motzkinInfo, int precision = 15, int maximumIteration = -1){
	cout << setprecision(precision);
	int i = 1;
	IterationVector old_vector(motzkinInfo, precision);
	while (1){
		i++;
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
// 		cout << "n="<<i<<endl<<"min = "<< min << endl << "max = " << max <<endl;
		if (mpf_eq(min.get_mpf_t(), max.get_mpf_t(), temp.get_prec()) || (maximumIteration > 0 && i > maximumIteration))
			return min;
		old_vector = new_vector;
	}
}

int main(int argc, char* argv[]){
	if (argc == 2 && strcmp(argv[1],"--v") == 0){
		cout << "GMP version: " << gmp_version << endl;
		exit(1);
	}
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
	cout << setprecision(precision) << printGrowthConstant(motzkinInfo,precision,maximumIteration) << endl;
	return 0;
}

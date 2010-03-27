#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ratio 1

int main () {
	FILE *write;
	int i, j, k, r;

	r = 18;

	write = fopen("out.txt", "w");

	for (i = 0-r; i <= r; i++) {
		k = sqrt((r * r) - (i * i)) * ratio;

		for (j = 0-r*ratio; j < r*ratio; j++) {
			if(j < 0) {
				if(abs(i) == 0 && abs(j) == r)
fprintf(write,"$");
				else if(abs(j) > k)
					fprintf(write," ");
				else if(abs(j) == k)
					fprintf(write,"@");
				else
					fprintf(write,"@");
			}
			else if(j > 0) {
				if(abs(i) == 0 && abs (j) ==r)
fprintf(write,"$");
				if (j < k)
					fprintf(write, "@");
				else if (j == k)
					fprintf(write, "@");
				else
					fprintf(write, " ");
			}
		}
		fprintf(write, "\n");
	}
	return 0;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ratio 15/7

int main () {
	FILE *write;
	int i, j, k, r;

	r = 20;

	write = fopen("out.txt", "w");

	for (i = 0-r; i <= r; i++) {
		k = sqrt((r * r) - (i * i)) * ratio;

		for (j = 0-r*ratio; j < r*ratio; j++) {
			if(j < 0) {
				if(abs(j) > k)
					fprintf(write," ");
				else if(abs(j) == k)
					fprintf(write,"0");
				else
					fprintf(write,"@");
			}
			else if(j > 0) {
				if (j < k)
					fprintf(write, "@");
				else if (j == k)
					fprintf(write, "0");
				else
					fprintf(write, " ");
			}
		}
		fprintf(write, "\n");
	}
	return 0;
}

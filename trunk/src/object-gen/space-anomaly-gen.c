#include <stdio.h>
#include <math.h>

//#define ratio 1.8

int main () {
   FILE *write;
   float ratio = 15/7;
   write = fopen("out.txt", "w");
   int i, j, k, r, outerK, x, prevK;
   r = 20;
   outerK = 1;
   for (i = 1-r; i <= r; i++) {
      k = sqrt((r * r) - (i * i)) * ratio;
      if (i == r - 1)
         prevK = k;
      if (i == r) {
         outerK = prevK * .7;
      }

      for (j = 0-r*ratio; j < r * ratio; j++) {
		if(j < 0) {
			if(abs(j) < r*ratio-k) {
				fprintf(write," ");
			}
			else if(abs(j) > r*ratio-k)
				fprintf(write,"@");
		}
		else if(j >= 0) {
         if (abs(j) < r*ratio-k) {
            fprintf(write, "@");
         }
         else if (k == 0) {
            for (x = 0; x < outerK; x++) {
               fprintf(write, "0");
            }
            k--;
         }
         else {
            fprintf(write, " ");
         }
		}
      }
      fprintf(write, "\n");
   }
   return 0;
}

#include <stdio.h>
#include <math.h>

//#define ratio 1.8

int main () {
   FILE *write;
   float ratio = 15/7;
   write = fopen("out.txt", "w");
   int i, j, k, r, outerK, x, prevK;
   int billybobjimboe;
   int billybobjimboe_tmp;
   r = 20;
   outerK = 1;
   for (i = 1-r; i <= r; i++) {
      k = sqrt((r * r) - (i * i)) * ratio;
      if (i == r - 1)
         prevK = k;
      if (i == r) {
         outerK = prevK * .7;
      }

    billybobjimboe_tmp = k;
	for(billybobjimboe = 0; billybobjimboe < 2; billybobjimboe++)
      for (j = 0; j < r * ratio; j++) {
        k = billybobjimboe_tmp;
		if(!billybobjimboe) {
			if(j < r*ratio-k) {
				fprintf(write," ");
			}
			else if(j > r*ratio-k)
				fprintf(write,"@");
		}
		else if(billybobjimboe) {
         if (j < r*ratio-k) {
            fprintf(write, "@");
            k--;
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

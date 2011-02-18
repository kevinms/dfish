#ifndef UTILS_H_
#define UTILS_H_

#define EN_BIG 0
#define EN_LIT 1

#define MAX(x,y) if(x>y) return x; else return y;
#define MIN(x,y) if(x<y) return x; else return y;

void print_bits32(int n);
int rand_max(int n);
int split_string(char *buf, char **seg);
char *p_strcpy(const char *str);
int search_match(char **tree, char *apple);
double getTime();
void die_with_error(char *error_message);
int fsize(const char *f);
int fcheck_for_user(const char *user);
int fcheck_for_file(const char *file);
char *fileFromPath(char * path);

#endif /* !UTILS_H_ */

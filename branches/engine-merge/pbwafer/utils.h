// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

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

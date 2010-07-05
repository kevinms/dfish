#ifndef __LOG_H
#define __LOG_H

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  define __func__ __FUNCTION__
# else
#  define __func__ "<unknown>"
# endif
#endif

#define LOG_e

GNU GCC/MinGW:
__FUNCTION__
__PRETTY_FUNCTION__

and more, like (standardized by ANSI C):
__LINE__
__FILE__
__TIME__
__DATE__

void LOG_Error();

#endif /* !__LOG_H */

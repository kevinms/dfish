/*
 * This file ONLY contains constants that NEED to be GLOBAL
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/* Define  space stuff to be 0-19 */
#define OBJ_STAR     0
#define OBJ_PLANET   1
#define OBJ_ASTEROID 2
#define OBJ_NEBULA   3

/* Define different types of units to be 20-49 */
#define OBJ_UNIT     20
#define OBJ_ENEMY    21
#define OBJ_CREATURE 22

/* Define different types of items to be 50-99 */
#define OBJ_ITEM 50

/* Define object classes */
#define A_CLASS 'A'
#define B_CLASS 'B'
#define C_CLASS 'C'

#define ratio 1
#define PI 3.141592653589793
#define BAD_LOC -1

/*************************************************************
 * dist * radius = minimum distance between objects
 * //later add dependencies on gravitational limitations//
 *************************************************************/
#define STAR_MINSAFEDIST   2
#define PLANET_MINSAFEDIST 1
#define AST_MINSAFEDIST    0.5

#endif /* !CONSTANTS_H_ */

#pragma once
#include <stdio.h>

//=========================================================
//TYPEDEFS
//=========================================================

typedef bool bit;
typedef unsigned char  byte;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

typedef const bit    cbit;
typedef const bool   cbool;
typedef const byte   cbyte;
typedef const char   cchar;
typedef const short  cshort;
typedef const int    cint;
typedef const long   clong;
typedef const float  cfloat;
typedef const double cdouble;
typedef const ushort cushort;
typedef const uint   cuint;
typedef const ulong  culong;

//=========================================================
//DEFINES
//=========================================================

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

//Numbers
#define N_XY 2
#define N_XYZ 3
#define N_RGB 3
#define N_RGBA 4
#define N_UV 2
#define N_PAIR 2
#define N_TRI 3
#define N_QUAD 4
#define N_QUAT 4
#define N_ROT 4
#define N_KILOBYTE 1024

//Offsets
#define O_X 0
#define O_Y 1
#define O_Z 2
#define O_R 0
#define O_G 1
#define O_B 2
#define O_A 3
#define O_U 0
#define O_V 1
#define O_QW 0
#define O_QX 1
#define O_QY 2
#define O_QZ 3
#define O_ANG 0
#define O_AX 1
#define O_AY 2
#define O_AZ 3

//=========================================================
//VARIABLES
//=========================================================

#if defined(_DEBUG)
cbool SphDebug = true;
#else
cbool SphDebug = false;
#endif

//=========================================================
//FUNCTIONS
//=========================================================

//Execute a debug break.
inline void DebugBreak(){
	__asm {
		int 3
	}
}

//Exchange two values.
template <typename T>
void Exchange(T& a, T& b){
	T temp = a;
	a = b;
	b = temp;
}

//=========================================================
//MACROS
//=========================================================

//Call delete on the object and set it to NULL.
#define DELETE(object)	\
	if(object != NULL){	\
		delete object;		\
		object = NULL;		\
	}

//Delete the array itself and set it to NULL.
#define DELETE_ARRAY(array)	\
	if(array != NULL){			\
		delete [] array;			\
		array = NULL;				\
	}

//Call delete [] on each element of the array.
//Delete the array itself and set it to NULL.
#define DELETE_ARRAY2(array, count) 		\
	if(array != NULL){							\
		for(int n = 0; n < count; n++){		\
			DELETE_ARRAY(array[n]);				\
		}												\
		delete [] array;							\
		array = NULL;								\
	}

//Call delete on each element of the array.
//Delete the array itself and set it to NULL.
#define DELETE_ARRAY_REC(array, count) 	\
	if(array != NULL){							\
		for(int n = 0; n < count; n++){		\
			DELETE(array[n]);						\
		}												\
		delete [] array;							\
		array = NULL;								\
	}

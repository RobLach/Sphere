#pragma once
#include "SphInclude.h"

//Concatenate str1 with str2 and return a new string.
//Note: the caller has to delete [] the result.
//return - new string = str1 + str2.
extern char* ConcatNew(cchar* str1, cchar* str2);

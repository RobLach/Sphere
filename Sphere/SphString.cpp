#include <cstring>
#include "SphString.h"

//Concatenate str1 with str2 and return a new string.
//Note: the caller has to delete [] the result.
//return - new string = str1 + str2.
char* ConcatNew(cchar* str1, cchar* str2){
	int str1Len = strlen(str1);
	int str12Len = str1Len + strlen(str2);
	char* result = new char[str12Len + 1];
	strcpy(result, str1);
	strcpy(result + str1Len, str2);
	result[str12Len] = '\0';
	return result;
}

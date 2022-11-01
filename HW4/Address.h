#pragma once

#define WORD_SEP "#"
#define PARAM_SEP "@"

char*	getAddress();

char*	getParam(const char* msg);
char*	fixAddressParam(char* param);
void	changeEvenWord(char* str);
char*	combineParams(char** strArr, int length, char* numStr);

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "General.h"
#include "Address.h"

char*	getAddress()
{
	//0 - country, 1 - city, 2 - street
	char* paramsArr[3];
	char* paramsArrStr[3] = { "country","city","street" };
	int num;
	char numStr[MAX_STR_LEN];

	printf("Enter airline address:\n");
	for (int i = 0; i < 3; i++)
	{
		paramsArr[i] = getParam(paramsArrStr[i]);
		if (!paramsArr[i])
			return NULL;
	}
	printf("Enter house number:\t");
	scanf("%d", &num);
	sprintf(numStr, "%d", num);

	char* address = combineParams(paramsArr, 3, numStr);

	for (int i = 0; i < 3; i++)
		free(paramsArr[i]);

	return address;
}


char*	getParam(const char* msg)
{
	char temp[MAX_STR_LEN];
	char* param;

	printf("Enter %s:\t", msg);
	myGets(temp, MAX_STR_LEN, stdin);

	param = fixAddressParam(temp);

	return param;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWords(param, &count, &totalLength);
	//add size for the WORD_SEP between words
	fixParamStr = (char*)calloc(totalLength + count, sizeof(char));
	if (!fixParamStr)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	if (count == 1)
	{
		wordsArray[0][0] = tolower(wordsArray[0][0]);
		strcpy(fixParamStr, wordsArray[0]);
	}
	else { //more than 1 word

		int len;
		for (int i = 0; i < count; i++)
		{
			len = (int)strlen(wordsArray[i]);
			if (len % 2 == 0) //even number of letters
				changeEvenWord(wordsArray[i]);
			else {
				if (i == count - 1) //last but not only!
					wordsArray[i][0] = tolower(wordsArray[i][0]); //small
				else
					wordsArray[i][0] = toupper(wordsArray[i][0]); //big
			}
			strcat(fixParamStr, wordsArray[i]);
			if (i != count - 1) //not last
				strcat(fixParamStr, WORD_SEP);
		}
	}
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return fixParamStr;

}

void	changeEvenWord(char* str)
{
	int index = 0;
	while (*str)
	{
		if (index % 2 == 0)
			*str = toupper(*str);
		else
			*str = tolower(*str);
		str++;
		index++;
	}
}

char*	combineParams(char** strArr, int length, char* numStr)
{
	char* address;
	//totalLength  - combine length + 3 PARAM_SEP + 1 '\0'
	size_t totalLength = 4 + strlen(numStr);
	for (int i = 0; i < length; i++)
		totalLength += strlen(strArr[i]);

	address = (char*)calloc(totalLength, sizeof(char));
	if (!address)
		return NULL;

	strcpy(address, strArr[0]);
	for (int i = 1; i < length; i++)
	{
		strcat(address, PARAM_SEP);
		strcat(address, strArr[i]);
	}
	strcat(address, PARAM_SEP);
	strcat(address, numStr);
	return address;
}
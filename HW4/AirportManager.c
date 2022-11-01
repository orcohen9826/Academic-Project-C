#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"

int		initManager(AirportManager* pManager, const char* fileName)
{
	if (loadManagerFromFile(pManager, fileName))
		return FROM_FILE;


	printf("------  Init airport Manager ----- User!!!\n");
	pManager->airportsCount = 0;
	pManager->airportsArr = NULL;
	char op;
	while (1)
	{
		printf("Do you want to enter an airport? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		if (!addAirport(pManager))
			return ERROR;
	}


	return FROM_USER;
}

int	addAirport(AirportManager* pManager)
{
	pManager->airportsArr = (Airport*)realloc(pManager->airportsArr, (pManager->airportsCount + 1) * sizeof(Airport));
	if (!pManager->airportsArr)
		return 0;

	initAirport(&pManager->airportsArr[pManager->airportsCount], pManager);
	
	pManager->airportsCount++;
	return 1;
}

void  initAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		pPort->name = getStrExactName("Enter airport name");
		if (checkUniqeName(pPort->name, pManager))
			break;

		printf("This name already in use - enter a different name\n");
		//free the memory is not good
		free(pPort->name);
	}

	initAirportNoName(pPort);
}

Airport* findAirportByName(const AirportManager* pManager, const char* name)
{
	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (isAirportName(&pManager->airportsArr[i], name))
			return &pManager->airportsArr[i];
	}
	return NULL;
}

int checkUniqeName(const char* name,const AirportManager* pManager)
{
	Airport* port = findAirportByName(pManager, name);

	if (port != NULL)
		return 0;

	return 1;
}

void	printAirports(const AirportManager* pManager)
{
	printf("there are %d airports\n", pManager->airportsCount);
	generalArrayFunction((void*)pManager->airportsArr, pManager->airportsCount, sizeof(Airport),
			printAriportV);
}


int	saveManagerToFile(const AirportManager* pManager, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "w");
	if (!fp) {
		printf("Error open airport manager file to write\n");
		return 0;
	}

	fprintf(fp, "%d\n", pManager->airportsCount);
	for(int i = 0; i< pManager->airportsCount; i++)
	{
		if (!saveAirportToFile(&pManager->airportsArr[i], fp))
		{
			printf("Error write airport\n");
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;

}

int		loadManagerFromFile(AirportManager* pManager, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf("Error open airport manager file\n");
		return 0;
	}

	fscanf(fp, "%d", &pManager->airportsCount);
	//clean the buffer
	fgetc(fp);
	pManager->airportsArr = (Airport*)malloc(pManager->airportsCount  * sizeof(Airport));
	if (!pManager->airportsArr)
		return 0;

	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (!loadAirportFromFile(&pManager->airportsArr[i], fp))
		{
			printf("Error loading airport from file\n");
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return 1;
}

void	freeManager(AirportManager* pManager)
{
	for (int i = 0; i < pManager->airportsCount; i++)
		freeAirport(&pManager->airportsArr[i]);
	free(pManager->airportsArr);
}

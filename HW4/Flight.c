#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "fileHelper.h"

void	initFlight(Flight* pFlight, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter name of origin airport:");
	pFlight->nameSource = _strdup(pPortOr->name);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter name of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	pFlight->nameDest = _strdup(pPortDes->name);
	initPlane(&pFlight->thePlane);
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceName(const Flight* pFlight, const char* nameSource)
{
	if (strcmp(pFlight->nameSource, nameSource) == 0)
		return 1;
		
	return 0;
}


int		isFlightToDestName(const Flight* pFlight, const char* nameDest)
{
	if (strcmp(pFlight->nameDest, nameDest) == 0)
		return 1;

	return 0;


}

int		isPlaneCodeInFlight(const Flight* pFlight, const char*  code)
{
	if (strcmp(pFlight->thePlane.code, code) == 0)
		return 1;
	return 0;
}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	if (pFlight->thePlane.type == type)
		return 1;
	return 0;
}


void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t",pFlight->nameSource, pFlight->nameDest);
	printDate(&pFlight->date);
	printPlane(&pFlight->thePlane);
}

void	printFlightV(const void* val)
{
	const Flight* pFlight = *(const Flight**)val;
	printFlight(pFlight);
}


Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char name[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		myGets(name, MAX_STR_LEN,stdin);
		port = findAirportByName(pManager, name);
		if (port == NULL)
			printf("No airport with this name - try again\n");
	} while(port == NULL);

	return port;
}

void	freeFlight(Flight* pFlight)
{
	free(pFlight->nameSource);
	free(pFlight->nameDest);
	free(pFlight);
}


int saveFlightToFile(const Flight* pF, FILE* fp)
{
	if (!writeStringToFile(pF->nameSource, fp, "Error write flight source name\n"))
		return 0;

	if (!writeStringToFile(pF->nameDest, fp, "Error write flight destination name\n"))
		return 0;

	if (!savePlaneToFile(&pF->thePlane,fp))
		return 0;

	if (!saveDateToFile(&pF->date,fp))
		return 0;

	return 1;
}


int loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp)
{

	pF->nameSource = readStringFromFile(fp, "Error reading source name\n");
	if (!pF->nameSource)
		return 0;

	pF->nameDest = readStringFromFile(fp, "Error reading destination name\n");
	if (!pF->nameDest)
	{
		free(pF->nameSource);
		return 0;
	}

	if (!checkSourceAndDestinationName(pManager, pF->nameSource, pF->nameDest))
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}

	if (!loadPlaneFromFile(&pF->thePlane, fp))
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}


	if (!loadDateFromFile(&pF->date, fp))
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}


	return 1;
}

int checkSourceAndDestinationName(const AirportManager* pManager, const char* src, const char* dest)
{

	if (findAirportByName(pManager, src) == NULL)
	{
		printf("Airport %s not in manager\n", src);
		return 0;
	}

	if (findAirportByName(pManager, dest) == NULL)
	{
		printf("Airport %s not in manager\n", dest);
		return 0;
	}
	return 1;
}


int		loadFlightFromFileCompress(Flight* pF, const AirportManager* pManager, FILE* fp)
{
	BYTE data[2];

	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	int lenSrc = data[0] >> 3;
	int lenDest = (data[0] & 0x7) << 2 | (data[1] >> 6);

	pF->nameSource = (char*)calloc(lenSrc + 1, sizeof(char));
	if (!pF->nameSource)
		return 0;

	pF->nameDest = (char*)calloc(lenDest + 1, sizeof(char));
	if (!pF->nameDest)
	{
		free(pF->nameSource);
		return 0;
	}

	if (!checkSourceAndDestinationName(pManager, pF->nameSource, pF->nameDest))
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}

	pF->thePlane.type = (data[1] >> 4) & 0x3;
	pF->date.month = data[1] & 0xF;

	BYTE nextData[3];
	if (fread(&nextData, sizeof(BYTE), 3, fp) != 3)
		return 0;
	pF->thePlane.code[0] = (nextData[0] >> 3) + 'A';
	pF->thePlane.code[1] = ( ((nextData[0] & 0x7) << 2) | ((nextData[1] >> 6) & 0x7) ) + 'A';
	pF->thePlane.code[2] = ((nextData[1] >> 1) & 0x1F) + 'A';
	pF->thePlane.code[3] = (((nextData[1] & 0x1) << 4) | (nextData[2] >> 4) & 0xF) + 'A';

	pF->date.year = (nextData[2] & 0xF) + MIN_YEAR;

	BYTE last;

	if (fread(&last, sizeof(BYTE), 1, fp) != 1)
		return 0;
	pF->date.day = last & 0x1F;

	if (fread(pF->nameSource, sizeof(char), lenSrc, fp) != lenSrc)
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}

	if (fread(pF->nameDest, sizeof(char), lenDest, fp) != lenDest)
	{
		free(pF->nameSource);
		free(pF->nameDest);
		return 0;
	}

	return 1;
}

int		saveFlightToFileCompress(const Flight* pF, FILE* fp)
{
	BYTE data[2];
	int lenSrc = (int)strlen(pF->nameSource);
	int lenDest = (int)strlen(pF->nameDest);

	data[0] = lenSrc << 3 | lenDest >> 2;
	data[1] = (lenDest & 0x3) << 6 | pF->thePlane.type << 4 | pF->date.month;

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	BYTE nextData[3];

	nextData[0] = (pF->thePlane.code[0] - 'A') << 3 | (pF->thePlane.code[1] - 'A') >> 2;
	nextData[1] =	((pF->thePlane.code[1] - 'A') & 0x3) << 6 |
					(pF->thePlane.code[2] - 'A') << 1 |
					(pF->thePlane.code[3] - 'A') >> 4;
	nextData[2] = ((pF->thePlane.code[3] - 'A') & 0xF) << 4 | (pF->date.year-MIN_YEAR);

	if (fwrite(&nextData, sizeof(BYTE), 3, fp) != 3)
		return 0;
	

	BYTE last = (BYTE)pF->date.day;

	if (fwrite(&last, sizeof(BYTE), 1, fp) != 1)
		return 0;
	

	if (fwrite(pF->nameSource, sizeof(char), lenSrc, fp) != lenSrc)
		return 0;

	if (fwrite(pF->nameDest, sizeof(char), lenDest, fp) != lenDest)
		return 0;


	return 1;
}

int	compareFlightBySourceName(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return strcmp(pFlight1->nameSource, pFlight2->nameSource);
}

int	compareFlightByDestName(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return strcmp(pFlight1->nameDest, pFlight2->nameDest);
}

int	compareFlightByPlaneCode(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;
	return strcmp(pFlight1->thePlane.code, pFlight2->thePlane.code);
}

int		compareFlightByDate(const void* flight1, const void* flight2)
{
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;


	return compareDate(&pFlight1->date, &pFlight2->date);
	

	return 0;
}


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"
#include "fileHelper.h"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Source Name", "Dest Name" , "Date", "Plane Code" };


int		initAirlineFromFile(Airline* pComp, AirportManager* pManaer, const char* fileName,int compress)
{
	L_init(&pComp->flighDateList);
	if (loadAirlineFromFile(pComp, pManaer, fileName, compress))
		return initDateList(pComp);

	return 0;
}

void	initAirline(Airline* pComp)
{
	printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	L_init(&pComp->flighDateList);
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if (pManager->airportsCount < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount+1) * sizeof(Flight*));
	if (!pComp->flightArr)
		return 0;
	pComp->flightArr[pComp->flightCount] = (Flight*)calloc(1,sizeof(Flight));
	if (!pComp->flightArr[pComp->flightCount])
		return 0;
	initFlight(pComp->flightArr[pComp->flightCount],pManager);

	int res = insertFlightDateToList(&pComp->flighDateList, pComp->flightArr[pComp->flightCount]);
	
	pComp->flightCount++;
	pComp->sortOpt = eNone; //new fight not sorted!
	return res;
}

void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("Has %d flights\n",pComp->flightCount);
	generalArrayFunction((void*)pComp->flightArr, pComp->flightCount, sizeof(Flight*), printFlightV);
	printf("\nFlight Date List:");
	L_print(&pComp->flighDateList, printDate);
}

void	doCountFlightsFromName(const Airline* pComp)
{
	if (pComp->flightCount == 0)
	{
		printf("No flight in company\n");
		return;
	}

	char* tempName = getStrExactName("Please enter origin airport name");

	int count = 0;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isFlightFromSourceName(pComp->flightArr[i], tempName))
			count++;
	}

	free(tempName);

	if (count != 0)
		printf("There are %d ", count);
	else
		printf("There are No ");

	printf("flights from this airport\n");
}

void	doPrintFlightsWithPlaneCode(const Airline* pComp)
{
	char code[MAX_STR_LEN];
	getPlaneCode(code);
	printf("All flights with plane code %s:\n",code);
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneCodeInFlight(pComp->flightArr[i], code))
			printFlight(pComp->flightArr[i]);
	}
	printf("\n");

}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();

	printf("All flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
			printFlight(pComp->flightArr[i]);
	}
	printf("\n");
}

int		saveAirlineToFile(const Airline* pComp, const char* fileName, int compress)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open copmpany file to write\n");
		return 0;
	}


	if (compress)
	{
		if (!saveBaseAirlineDataCompress(pComp, fp))
			return 0;
	}
	else {
		if (!saveBaseAirlineData(pComp, fp))
			return 0;
	}


	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (compress)
		{
			if (!saveFlightToFileCompress(pComp->flightArr[i], fp))
				return 0;
		}
		else {
			if (!saveFlightToFile(pComp->flightArr[i], fp))
				return 0;
		}
	}

	fclose(fp);
	return 1;
}

int		saveBaseAirlineData(const Airline* pComp, FILE* fp)
{
	if (!writeStringToFile(pComp->name, fp, "Error write comapny name\n"))
		return 0;

	if (!writeIntToFile((int)pComp->sortOpt, fp, "Error write sort option\n"))
		return 0;

	if (!writeIntToFile(pComp->flightCount, fp, "Error write flight count\n"))
		return 0;
	
	return 1;
}

int		saveBaseAirlineDataCompress(const Airline* pComp, FILE* fp)
{
	BYTE data[2];
	int len = (int)strlen(pComp->name);
	data[0] = pComp->flightCount >> 1;
	data[1] = pComp->flightCount << 7 | pComp->sortOpt << 4 | len;

	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	if (fwrite(pComp->name, sizeof(char), len, fp) != len)
		return 0;

	return 1;
}


int		loadAirlineFromFile(Airline* pComp, const AirportManager* pManager,const char* fileName, int compress)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	pComp->flightArr = NULL;

	if (compress)
	{
		if (!readBaseAirlineDataCompress(pComp, fp))
		{
			fclose(fp);
			return 0;
		}
	}
	else {
		if (!readBaseAirlineData(pComp, fp))
		{
			fclose(fp);
			return 0;
		}
	}


	
	if (pComp->flightCount > 0)
	{
		pComp->flightArr = (Flight**)malloc(pComp->flightCount * sizeof(Flight*));
		if (!pComp->flightArr)
		{
			printf("Alocation error\n");
			fclose(fp);
			return 0;
		}
	}
	else
		pComp->flightArr = NULL;

	for (int i = 0; i < pComp->flightCount; i++)
	{
		pComp->flightArr[i] = (Flight*)calloc(1, sizeof(Flight));
		if (!pComp->flightArr[i])
		{
			printf("Alocation error\n");
			fclose(fp);
			return 0;
		}

		if (compress)
		{
			if (!loadFlightFromFileCompress(pComp->flightArr[i], pManager, fp))
			{
				fclose(fp);
				return 0;
			}

		}
		else {
			if (!loadFlightFromFile(pComp->flightArr[i], pManager, fp))
			{
				fclose(fp);
				return 0;
			}

		}
	}

	fclose(fp);
	return 1;
}

int		readBaseAirlineData(Airline* pComp, FILE* fp)
{
	pComp->name = readStringFromFile(fp, "Error reading company name\n");
	if (!pComp->name)
		return 0;

	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
		return 0;

	pComp->sortOpt = (eSortOption)opt;

	if (!readIntFromFile(&pComp->flightCount, fp, "Error reading flight count name\n"))
		return 0;

	return 1;
}

int		readBaseAirlineDataCompress(Airline* pComp, FILE* fp)
{
	BYTE data[2];

	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	pComp->flightCount = data[0] << 1 | (data[1] >> 7) & 0x1;
	pComp->sortOpt = (data[1] >> 4) & 0x7;

	int len = data[1] & 0xF;

	pComp->name = (char*)calloc(len + 1, sizeof(char));
	if (!pComp->name)
		return 0;

	if (fread(pComp->name, sizeof(char), len, fp) != len)
	{
		free(pComp->name);
		return 0;
	}
	return 1;
}

void	sortFlight(Airline* pComp)
{
	pComp->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pComp->sortOpt)
	{
	case eSourceName:
		compare = compareFlightBySourceName;
		break;
	case eDestName:
		compare = compareFlightByDestName;
		break;
	case eDate:
		compare = compareFlightByDate;
		break;
	case ePlaneCode:
		compare = compareFlightByPlaneCode;
		break;

	}

	if (compare != NULL)
		qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
}

void	findFlight(const Airline* pComp)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Flight f = { 0 };
	Flight* pFlight = &f;


	switch (pComp->sortOpt)
	{
	case eSourceName:
		f.nameSource = getStrExactName("Source airport name?");
		compare = compareFlightBySourceName;
		break;

	case eDestName:
		f.nameDest = getStrExactName("Destination airport name?");
		compare = compareFlightByDestName;
		break;
	
	case eDate:
		getCorrectDate(&f.date);
		compare = compareFlightByDate;
		break;

	case ePlaneCode:
		getPlaneCode(f.thePlane.code);
		compare = compareFlightByPlaneCode;
		break;
	}

	if (compare != NULL)
	{
		Flight** pF = bsearch(&pFlight, pComp->flightArr, pComp->flightCount, sizeof(Flight*), compare);
		if (pF == NULL)
			printf("Flight was not found\n");
		else {
			printf("Flight found, ");
			printFlight(*pF);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}

	if (f.nameSource != NULL)
		free(f.nameSource);

	if (f.nameDest != NULL)
		free(f.nameDest);
}

int	initDateList(Airline* pComp)
{
	if (pComp->flightCount == 0) //no flights!!!
		return 1;

	for (int i = 1; i < pComp->flightCount; i++)
	{
		if (!insertFlightDateToList(&pComp->flighDateList, pComp->flightArr[i]))
			return 0;
	}
	return 1;
}

int insertFlightDateToList(LIST* lst, Flight* pFlight)
{
	int compareRes = 0;

	Date* pDate = &pFlight->date;
	NODE* pNode = &(lst->head);
	
	while (pNode->next != NULL)
	{
		compareRes = compareDate(pDate, pNode->next->key);
		if (compareRes <= 0) // found the place to inset!!
			break;

		pNode = pNode->next;
	}

	if (compareRes < 0  || pNode->next == NULL) //equal will not be inserted!!!
		pNode = insertDateToList(pNode, pDate);

	if (!pNode)
		return 0;
	return 1;
}

NODE* insertDateToList(NODE* pNode, Date* pDate)
{
	Date* pNewDate = (Date*)malloc(sizeof(Date));
	if (!pNewDate)
		return NULL;
	*pNewDate = *pDate;
	
	return  L_insert(pNode, pNewDate);
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		freeFlight(arr[i]);
	}
}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->name);
	//free the Date* in the list
	L_free(&pComp->flighDateList, freeDate);
}
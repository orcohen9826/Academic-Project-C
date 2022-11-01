#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Airport.h"
#include "General.h"
#include "Address.h"


int	isSameAirport(const Airport* pPort1, const Airport* pPort2)
{
	if (!pPort1 || !pPort2)
		return 0;
	if (strcmp(pPort1->name, pPort2->name) == 0)
		return 1;

	return 0;
}

int	isAirportName(const Airport* pPort1, const char* name)
{
	if (!pPort1)
		return 0;
	if (strcmp(pPort1->name, name) == 0)
		return 1;

	return 0;
}

void	initAirportNoName(Airport* pPort)
{
	//only address!!!
	pPort->address = getAddress();
}

void	printAirport(const Airport* pPort)
{
	printf("Airport name:%-20s\t", pPort->name);
	printf("Airport address: %s\n", pPort->address);
}

void	printAriportV(const void* pP)
{
	const Airport* pPort = (const Airport*)pP;
	printAirport(pPort);
}

void	freeAirport(Airport* pPort)
{
	free(pPort->name);
	free(pPort->address);
}

int		saveAirportToFile(const Airport* pAir, FILE* fp)
{
	if (!pAir)
		return 0;
	fprintf(fp, "%s\n", pAir->name);
	fprintf(fp, "%s\n", pAir->address);
	
	return 1;
}

int		loadAirportFromFile(Airport* pAir, FILE* fp)
{
	char temp[MAX_STR_LEN];
	if (!pAir)
		return 0;
	
	myGets(temp, MAX_STR_LEN, fp);
	pAir->name = getDynStr(temp);
	
	myGets(temp, MAX_STR_LEN, fp);
	pAir->address = getDynStr(temp);
	return 1;

}

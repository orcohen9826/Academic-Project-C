#ifndef __AIRPORT__
#define __AIRPORT__

#include "General.h"

typedef struct
{
	char*	name;
	char*	address;
}Airport;

int		isSameAirport(const Airport* pPort1, const Airport* pPort2);
int		isAirportName(const Airport* pPort1, const char* name);
void	initAirportNoName(Airport* pPort);
void	printAirport(const Airport* pPort);
void	printAriportV(const void* pPort);
void	freeAirport(Airport* pPort);

int		saveAirportToFile(const Airport* pAir, FILE* fp);
int		loadAirportFromFile(Airport* pAir, FILE* fp);

#endif
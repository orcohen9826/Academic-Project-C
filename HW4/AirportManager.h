#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "GeneralList.h"

#define ERROR 0
#define FROM_FILE 1
#define FROM_USER 2

typedef struct
{
	Airport*	airportsArr;
	int			airportsCount;
}AirportManager;

int		initManager(AirportManager* pManager, const char* fileName); 
int		addAirport(AirportManager* pManager);
void	initAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByName(const AirportManager* pManager, const char* name);
int		checkUniqeName(const char* name, const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);

int		saveManagerToFile(const AirportManager* pManager, const char* fileName);
int		loadManagerFromFile(AirportManager* pManager, const char* fileName);

void	freeManager(AirportManager* pManager);
#endif
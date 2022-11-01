#ifndef __FLIGHT__
#define __FLIGHT__


#include "AirportManager.h"
#include "Date.h"
#include "General.h"
#include "Plane.h"

typedef struct
{
	char*		nameSource;
	char*		nameDest;
	Plane		thePlane;
	Date		date;
}Flight;

void	initFlight(Flight* pFlight,const AirportManager* pManager);
int		isFlightFromSourceName(const Flight* pFlight, const char* nameSource);
int		isFlightToDestName(const Flight* pFlight, const char* nameDest);
void	printFlight(const Flight* pFlight);
void	printFlightV(const void* val);
Airport*	setAiportToFlight(const AirportManager* pManager, const char* msg);
int		isPlaneCodeInFlight(const Flight* pFlight, const char*  code);
int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type);
void	freeFlight(Flight* pFlight);

int		loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp);
int		saveFlightToFile(const Flight* pF, FILE* fp);

int		loadFlightFromFileCompress(Flight* pF, const AirportManager* pManager, FILE* fp);
int		saveFlightToFileCompress(const Flight* pF, FILE* fp);

int		checkSourceAndDestinationName(const AirportManager* pManager, const char* src, const char* dest);

int		compareFlightBySourceName(const void* air1, const void* air2);
int		compareFlightByDestName(const void* air1, const void* air2);
int		compareFlightByPlaneCode(const void* air1, const void* air2);
int		compareFlightByDate(const void* air1, const void* air2);

#endif

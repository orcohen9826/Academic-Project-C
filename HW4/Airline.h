#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"

typedef enum { eNone, eSourceName, eDestName,eDate, ePlaneCode,eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];


typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	eSortOption	sortOpt;
	LIST		flighDateList;
}Airline;

int		initAirlineFromFile(Airline* pComp, AirportManager* pManaer, const char* fileName, int compress);
void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp,const AirportManager* pManager);
void	printCompany(const Airline* pComp);
void	doCountFlightsFromName(const Airline* pComp);
void	doPrintFlightsWithPlaneCode(const Airline* pComp);
void	doPrintFlightsWithPlaneType(const Airline* pComp);

int		saveAirlineToFile(const Airline* pComp, const char* fileName, int compress);
int		loadAirlineFromFile(Airline* pComp, const AirportManager* pManager, const char* fileName, int compress);
int		readBaseAirlineData(Airline* pComp,FILE* fp);
int		saveBaseAirlineData(const Airline* pComp, FILE* fp);

int		saveBaseAirlineDataCompress(const Airline* pComp, FILE* fp);
int		readBaseAirlineDataCompress(Airline* pComp, FILE* fp);


void	sortFlight(Airline* pComp);
void	findFlight(const Airline* pComp);

int		initDateList(Airline* pComp);
int		insertFlightDateToList(LIST* lst, Flight* pFlight);
NODE*	insertDateToList(NODE* pNode,Date* pDate);

void	freeFlightArr(Flight** arr, int size);
eSortOption showSortMenu();
void	freeCompany(Airline* pComp);

#endif


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "General.h"
#include "Plane.h"

static const char* PlaneTypeStr[eNofPlaneTypes]
		= { "Commercial", "Cargo", "Military" };

void initPlane(Plane* pPlane)
{
	pPlane->type = getPlaneType();
	getPlaneCode(pPlane->code);
}


ePlaneType getPlaneType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofPlaneTypes; i++)
			printf("%d for %s\n", i, PlaneTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofPlaneTypes);
	getchar();
	return (ePlaneType) option;
}

const char*		GetPlaneTypeStr(int type)
{
	if (type < 0 || type >= eNofPlaneTypes)
		return NULL;
	return PlaneTypeStr[type];
}

void getPlaneCode(char* code)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter plane code  - %d UPPER CASE letters\t", CODE_LENGTH);
		myGets(temp, MAX_STR_LEN, stdin);
		if (strlen(temp) != CODE_LENGTH)
		{
			printf("code should be %d letters\n", CODE_LENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < CODE_LENGTH; i++)
			{
				if (isupper(temp[i]) == 0)
				{
					printf("Need to be upper case letter\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

void	printPlane(const Plane* pPlane)
{
	printf("Plane: type %s, code %s\n", PlaneTypeStr[pPlane->type], pPlane->code);

}

int		savePlaneToFile(const Plane* pPlane, FILE* fp)
{
	if (fwrite(pPlane, sizeof(Plane), 1, fp) != 1)
	{
		printf("Error write date\n");
		return 0;
	}

	return 1;

}

int		loadPlaneFromFile(Plane* pPlane, FILE* fp)
{
	if (fread(pPlane, sizeof(Plane), 1, fp) != 1)
	{
		printf("Error write date\n");
		return 0;
	}

	return 1;

}
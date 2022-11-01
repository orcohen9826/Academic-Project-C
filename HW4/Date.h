#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2021

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);

int		compareDate(const void* d1, const void* d2);
void	printDate(const void* d);
void	freeDate(void* d);


int		saveDateToFile(const Date* pDate, FILE* fp);
int		loadDateFromFile(Date* pDate, FILE* fp);
#endif

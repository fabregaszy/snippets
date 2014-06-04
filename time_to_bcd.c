#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BCDTODEC(b)     ((b)/16*10+(b)%16)
#define DECTOBCD(d)     ((d)/10*16+(d)%10)

void time_to_bcd(const char *sectime, char *bcdtime)
{
    time_t lsectime;
    struct tm *ptm;
    int year_high, year_low;
    int mon, day, hour, min, sec;

    lsectime = (time_t)strtol(sectime, NULL, 0);
    ptm = localtime(&lsectime);

    year_high = 19 + ptm->tm_year/100;
    year_low  = ptm->tm_year%100;
    mon  = ptm->tm_mon + 1;
    day  = ptm->tm_mday;
	hour = ptm->tm_hour;
	min  = ptm->tm_min;
	sec  = ptm->tm_sec;

	/** for debug
    printf("Year: %d-%d, Month: %d, Day: %d\n", year_high, year_low, mon, day);
    printf("Hour: %d, Min: %d, Sec: %d\n", hour, min, sec);
	*/

	*bcdtime = (char)DECTOBCD(year_high);
	*(bcdtime + 1) = (char)DECTOBCD(year_low);
	*(bcdtime + 2) = (char)DECTOBCD(mon);
	*(bcdtime + 3) = (char)DECTOBCD(day);
	*(bcdtime + 4) = (char)DECTOBCD(hour);
	*(bcdtime + 5) = (char)DECTOBCD(min);
	*(bcdtime + 6) = (char)DECTOBCD(sec);

}


int main(void)
{
    char* test = "1401891805";
    char bcdtime[8];
	int i;

    time_to_bcd(test, bcdtime);
	for(i = 0; i < 8; i++)
	{
		printf("bcdtime[%d] --> %X\n", i, bcdtime[i]);
	}

	test = "0";
    time_to_bcd(test, bcdtime);
	for(i = 0; i < 8; i++)
	{
		printf("bcdtime[%d] --> %X\n", i, bcdtime[i]);
	}

}



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BCDTODEC(b)     ((b)/16*10+(b)%16)

void bcd_to_dec_time(unsigned char *bcd_data)
{
	struct tm stm;

	stm.tm_year =  (BCDTODEC(bcd_data[0]) - 19) * 100;
    stm.tm_year += BCDTODEC(bcd_data[1]);
    stm.tm_mon  =  BCDTODEC(bcd_data[2]) - 1;
    stm.tm_mday =  BCDTODEC(bcd_data[3]);

	printf("Year: %d, Month: %d, Day: %d\n", stm.tm_year, stm.tm_mon, stm.tm_mday);
}

int main(void)
{
	char wk_time[20];
	char date[] = "2014-05-27";
	int i = 0;
	int cnt  = 0;
	int bcnt = 0;

	memset(wk_time, 0, 20);

	/* "YYYY-MM-DD" -> 0xYYYYMMDD */
	memset(wk_time, 0, 20);
	while(cnt < 10)
	{
    	wk_time[bcnt] = (char)(((date[cnt] & 0x0F) << 4) | (date[cnt + 1] & 0x0F));
		bcnt++;
    	if(cnt == 0)
    	{
        	cnt += 2;
    	}
    	else
    	{
        	cnt += 3; /* skip '-' between two numbers */
    	}
	} 
	for(i = 0; i < 10; i++)
	{
		printf("wk_time --> %x\n", wk_time[i]);
	}
	bcd_to_dec_time((unsigned char*)wk_time);

	return 0;
}

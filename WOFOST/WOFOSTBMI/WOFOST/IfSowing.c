#include <stdio.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function void IfSowing    ()                                              */
/*  Purpose: Checks whether sowing has occurred. Note that if the emergence   */
/*           flag is set to 1 the crop simulation starts the next day. If it  */
/*           is set to 0 the Emergence date has to be established.            */
/* ---------------------------------------------------------------------------*/
size_t Day;

void IfSowing()
{
    int month = Planting_date[Lat][Lon] ;
    //int month = 11;
    int day = 1;
    printf("check %d\n", Planting_date[Lat][Lon]);   
    if ( current_date.tm_mon == (month-1) &&
        current_date.tm_mday == day && 
        MeteoYear[Day] <= Meteo->EndYear)                
    {
        Crop->Sowing = 1;
        printf("check %d\n", Planting_date[Lat][Lon]);
        printf("month %d\n", current_date.tm_mon);
        fprintf(stderr,"%d\n",current_date.tm_mday);
        fprintf(stderr,"%d\n",day);
        //exit(1);
        
    }
 }





/*void IfSowing()
{
    int date = Planting_date[Lat][Lon] ;
    int month = 0;
    int day = 0; 
    int daysInMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int i = 0; i < 12; i++) {
        if (date <= daysInMonth[i]) {
            month = i + 1;
            day = date;
            break;
        }
        date -= daysInMonth[i];
    }
       
    if ( current_date.tm_mon == (month-1) &&
        current_date.tm_mday == day && 
        MeteoYear[Day] <= Meteo->EndYear)                
    {
        Crop->Sowing = 1;
        printf("check %d\n", date);
        fprintf(stderr,"%d\n",date);
        
    }
 }*/


/*void IfSowing(char* dateString)
{
    int month, start_day;
     
    sscanf(dateString, "%d-%d", &month, &start_day);
    
      
    if (current_date.tm_mon == (month-1) &&
        current_date.tm_mday == start_day && 
        MeteoYear[Day] <= Meteo->EndYear)
                
    {
        Crop->Sowing = 1;
    }
 }*/

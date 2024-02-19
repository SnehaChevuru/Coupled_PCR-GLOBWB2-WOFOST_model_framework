#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function List()                                                           */
/*  Purpose: Get the value of a user provided input table                     */
/* ---------------------------------------------------------------------------*/

struct tm current_date;
Weather *Meteo;

float List(TABLE_D *Table)
{
    
    //extern dmy_struct *dmy;
    //extern size_t   current;
    int month = Planting_date[Lat][Lon] ;
    
     while (Table) 
    { 
        
		int dates[] = {10,30};
		for (int i = 0; i < 2; i++){
			int day = dates[i];
		
			if (current_date.tm_mon == (month-1) &&
				current_date.tm_mday == day && 
				MeteoYear[Day] <= Meteo->EndYear)
			 
			{
				return Table->amount;
			}
			Table = Table->next;
		}
    }
    return 0.;
}



/*float List(TABLE_D *Table)
{
    
    //extern dmy_struct *dmy;
    //extern size_t   current;
    
     while (Table) 
    { 
        
		int dates[] = {10,30};
		for (int i = 0; i < 2; i++){
			int date = Planting_date[Lat][Lon]+ dates[i] ;
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
		
		
			if (current_date.tm_mon == (month -1) &&
				current_date.tm_mday == day && 
				MeteoYear[Day] <= Meteo->EndYear)
			 
			{
				return Table->amount;
			}
			Table = Table->next;
		}
    }
    return 0.;
}*/


/*float List(TABLE_D *Table)
{
    
    //extern dmy_struct *dmy;
    //extern size_t   current;
    
     while (Table) 
    { 
        if (current_date.tm_mon == Table->month -1 &&
            current_date.tm_mday == Table->day && 
            MeteoYear[Day] <= Meteo->EndYear)           
       
         
        {
            return Table->amount;
        }
        Table = Table->next;
    }
    
    return 0.;
}*/


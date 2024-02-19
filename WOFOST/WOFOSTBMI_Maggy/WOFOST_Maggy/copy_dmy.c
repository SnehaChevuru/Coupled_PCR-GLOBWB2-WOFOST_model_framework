#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <netcdf.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"



/******************************************
* @brief   Copy dmy structure
******************************************/
void
copy_dmy(dmy_struct *from,
         dmy_struct *to)
{
    to->day = from->day;
    to->day_in_year = from->day_in_year;
    to->dayseconds = from->dayseconds;
    to->month = from->month;
    to->year = from->year;
}

/******************************************
* @brief   Initialize dmy structure
******************************************/
void
initialize_dmy(dmy_struct *dmy)
{
    dmy->day = 0;
    dmy->day_in_year = 0;
    dmy->dayseconds = 0;
    dmy->month = 0;
    dmy->year = 0;
}


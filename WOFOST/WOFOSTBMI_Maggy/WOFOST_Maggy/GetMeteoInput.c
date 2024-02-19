#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* --------------------------------------------------------------------------*/
/*  function GetMeteoInput()                                                 */
/*  Purpose: Get the names of meteo files and the start and end year of the  */
/*           of the simulations. Read the la and lon of the boxes. Store the */
/*           information of each cell in the Meteo struct.                   */
/* --------------------------------------------------------------------------*/

int GetMeteoInput(char *list)
{
    FILE *ifp;
    
    Weather *initial = NULL;
    int i;
    
    int Emergence;
    int k;
    
    
    char output[MAX_STRING];
    char start[MAX_STRING];
    char cf[MAX_STRING], sf[MAX_STRING], mf[MAX_STRING], site[MAX_STRING];
    
    char line[MAX_STRING];
    unsigned int type;
    int StartYear;
    int EndYear;
    char path[MAX_STRING];
    char mask[MAX_STRING];
    char planting_date[MAX_STRING];
    char filename[MAX_STRING];
    char filetype[MAX_STRING];
    char varname[MAX_STRING];
    
   
    if ((ifp = fopen(list, "r")) == NULL) 
    {
        fprintf(stderr, "Can't open input, %s\n", list);
        exit(1);
        
    }
    
    k = 0;
    
    while(fgets(line, MAX_STRING, ifp)) {
                     
        sscanf(line,"%s %s %s %s %s %s %d %s\n" ,
            path, cf, sf, mf, site, start, &Emergence, output);
    
        k = GetSimInput(path, cf, sf, mf, site, start, Emergence, output,k);
        k++;       
        
        if(fgets(line, MAX_STRING, ifp) == NULL){
                fprintf(stderr, "Missing mask and year\n");
                exit(1);
            }
                               
        sscanf(line,"%s %d %d %s" , path, &StartYear, &EndYear, mask);
        
        if (initial == NULL) 
        {
            Meteo = initial = malloc(sizeof(Weather));
        }
        else 
        {
            Meteo->next = malloc(sizeof(Weather));
            Meteo = Meteo->next;  
        }  

        memset(Meteo->mask,'\0',MAX_STRING);
        strncpy(Meteo->mask, mask, strlen(mask));
        Meteo->StartYear = StartYear;
        Meteo->EndYear = EndYear;
        Meteo->next = NULL;
        
        if(fgets(line, MAX_STRING, ifp) == NULL){
                fprintf(stderr, "Missing crop calendar\n");
                exit(1);
            }
                  
        sscanf(line,"%s " , planting_date);
        
        memset(Meteo->planting_date, '\0',MAX_STRING);
        strncpy(Meteo->planting_date, planting_date, strlen(planting_date));
        
        
        for (i = 0; i < WEATHER_NTYPES; i++) {
            if(fgets(line, MAX_STRING, ifp) == NULL){
                fprintf(stderr, "Missing meteo types\n");
                exit(1);
            }
                        
            sscanf(line,"%s %s %s" , filename, filetype, varname);
                    
            if (strlen(filename) >= MAX_STRING) exit(0);

            if (strcmp(filetype,"TMIN") == 0) {
                type = WEATHER_TMIN;
            } else if (strcmp(filetype, "TMAX") == 0) {
                type = WEATHER_TMAX;
            } else if (strcmp(filetype, "RADIATION") == 0) {
                type = WEATHER_RADIATION;
            } else if (strcmp(filetype, "RAIN") == 0) {
                type = WEATHER_RAIN;
            } else if (strcmp(filetype, "WINDSPEED") == 0) {
                type = WEATHER_WINDSPEED;
            } else if (strcmp(filetype, "VAPOUR") == 0) {
                type = WEATHER_VAPOUR;
            } else {
                fprintf(stderr, "Unknown meteo type %s\n", filetype);
                exit(1);
            }
            memset(Meteo->type[type],'\0',MAX_STRING);
            strncpy(Meteo->type[type], filetype, strlen(filetype));

            memset(Meteo->file[type],'\0',MAX_STRING);
            strncpy(Meteo->file[type], path, strlen(path));
            strncat(Meteo->file[type], filename, strlen(filename));
            memset(Meteo->var[type],'\0',MAX_STRING);
            strncpy(Meteo->var[type], varname, strlen(filename));
        }
        
    }
          
    Meteo = initial;
    fclose(ifp);
    
    return k;
  
}

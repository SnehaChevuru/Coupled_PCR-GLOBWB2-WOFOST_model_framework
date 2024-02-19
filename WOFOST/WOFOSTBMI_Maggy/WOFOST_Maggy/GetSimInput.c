#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ----------------------------------------------------------------------------*/
/*  function GetMeteoInput()                                                   */
/*  Purpose: Get the names of the crop, soil, management and site files as     */
/*           as well if the simulation starting day and whether the simulation */
/*           starts at emergence (1) or sowing (0)                             */
/*-----------------------------------------------------------------------------*/




int GetSimInput(char *path, char *cf, char *sf, char *mf, char *site, char *start, int Emergence, char *output, int count)
{
    
       
    Input *ini = NULL; 
     
    if (ini == NULL) 
        {
            data = ini = malloc(sizeof(Input));
        }
        else 
        {
            data->next = malloc(sizeof(Input));
            data = data->next;  
        }  

    
    memset(data->cropfile,'\0',MAX_STRING);
    memset(data->sitefile,'\0',MAX_STRING);
    memset(data->soilfile,'\0',MAX_STRING);
    memset(data->management,'\0',MAX_STRING);

    strncpy(data->cropfile, path, strlen(path));
    strncat(data->cropfile, cf, strlen(cf));

    strncpy(data->soilfile, path, strlen(path));
    strncat(data->soilfile, sf, strlen(sf));

    strncpy(data->management, path, strlen(path));
    strncat(data->management, mf, strlen(mf));

    strncpy(data->sitefile, path, strlen(path));
    strncat(data->sitefile, site, strlen(site));
    
    
    
    if (strlen(sf) >= MAX_STRING) exit(0);
    if (strlen(output) >= MAX_STRING) exit(0);    
    if (strlen(start) >= MAX_STRING) exit(0);  

        
    memset(data->output,'\0',MAX_STRING);
    memset(data->start,'\0',MAX_STRING);

    strncpy(data->output,output,strlen(output)); // Name of output file
    strncpy(data->start, start,strlen(start)); // Starting string month day of the simulations 
    
    
    data->file = count;      // number of elements in Grid carousel
    //printf("runn %d \n",data->file);
    data->emergence = Emergence;
    data->next =NULL;
   
    GetInputData(data);   
       
    data = ini;
    /* Return the number of meteo files */
    return count;
}  

SimUnit *Grid;
Input *data;

int GetInputData(Input* data)
{
    SimUnit *initial =NULL;
   
    if (initial == NULL) 
    {
        Grid = initial =  malloc(sizeof(SimUnit));
    }
    else 
    {
        Grid->next = malloc(sizeof(SimUnit));
        Grid = Grid->next;  
    }
         
       
    GetCropData(Grid->crp   = malloc(sizeof(Plant)), data->cropfile); 
    GetSiteData(Grid->ste   = malloc(sizeof(Field)), data->sitefile);
    GetManagement(Grid->mng = malloc(sizeof(Management)), data->management);
    GetSoilData(Grid->soil  = malloc(sizeof(Soil)), data->soilfile);
    
       
    strncpy(Grid->output,data->output,strlen(data->output)); // Name of output file
    strncpy(Grid->start, data->start,strlen(data->start)); 
  
    
    Grid->file  = data->file;            // number of elements in Grid carousel
    //printf("runn %d \n",Grid->file);
    //strpdmy(start, "%m-%d", &Grid->start);
   
    Grid->emergence = data->emergence;      // Start the simulations at emergence (1) or at sowing (0)                
    Grid->crp->Sowing = 0;
    Grid->crp->Emergence = 0;         // Crop emergence has not yet occurred
    Grid->next = NULL;
    
        
    /*Set Grid back to initial address */
    Grid = initial;
    
    return Grid->file;
}

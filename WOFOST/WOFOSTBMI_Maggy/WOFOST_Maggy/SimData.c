#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <netcdf.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

int GetSimData(Weather* meteo, SimUnit* templateGrid) 
{
    size_t j, k;
    size_t lat_length, lon_length;
    //size_t time_length;
    SimUnit* templateInitial;
    SimUnit* cellGrid;
    //float *Grids[lon_length][lat_length];
    
    
    lat_length = meteo->nlat;
    lon_length = meteo->nlon;
    
    //time_length = meteo->ntime;
    templateInitial= templateGrid ;
    
    Grids = malloc(lat_length * sizeof(*Grids));
    if(Grids == NULL){
        fprintf(stderr, "Could not malloc");
        exit(1); 
    }
    for (j = 0; j < lat_length; j++) {
        Grids[j] = malloc(lon_length * sizeof(*Grids[j]));
        if(Grids[j] == NULL){
            fprintf(stderr, "Could not malloc");
            exit(1); 
        }
        
        for (k = 0; k < lon_length; k++) {
            Grids[j][k] = NULL;
        }
        
        for (k = 0; k < lon_length; k++) {
            //Grids[j][k] = malloc(time_length * sizeof(*Grids[j][k]));
            //Grids[j][k] = malloc(sizeof(SimUnit));
            templateGrid = templateInitial;
            //cellGrid = malloc(sizeof(SimUnit));
             
            
            while(templateGrid) {
                // Loop through all template crops and allocate memory
                if(Grids[j][k] == NULL) {
                    Grids[j][k] = malloc(sizeof(SimUnit));
                } else {
                    Grids[j][k]->next = malloc(sizeof(SimUnit));
                    Grids[j][k] = Grids[j][k]->next;
                }
                Grids[j][k]->crp   = malloc(sizeof(Plant));
                Grids[j][k]->ste   = malloc(sizeof(Field));
                Grids[j][k]->mng = malloc(sizeof(Management));
                Grids[j][k]->soil  = malloc(sizeof(Soil));
                Grids[j][k]->next = NULL;
                
                templateGrid = templateGrid->next;
            }
        }
    }
    
   
    //filling
    for (j = 0; j < lat_length; j++) {
        for (k = 0; k < lon_length; k++) {
            
            cellGrid = Grids[j][k];
            templateGrid = templateInitial;
            
            while(templateGrid) {
                copy_wofost_grid(templateGrid ,cellGrid, j,k );
                  
                templateGrid = templateGrid->next;
                cellGrid = cellGrid-> next;
            }
        }
    }
    
    templateGrid = templateInitial;
    //Clean(templateGrid);
    
    return 1;
}

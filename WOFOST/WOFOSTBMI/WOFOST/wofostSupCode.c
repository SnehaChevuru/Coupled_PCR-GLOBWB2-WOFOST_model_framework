#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define CHAR_CAST (char *)
#else
#define CHAR_CAST
#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"
#include "penman.h"
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "bmi.h"
#include "wofost_bmi.h"

#define INPUT_VAR_NAME_COUNT 9
#define OUTPUT_VAR_NAME_COUNT 13

/* set decimals */
#define roundz(x,d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))

static const char *output_var_names[OUTPUT_VAR_NAME_COUNT] = {
    "soil_moisture",
    "PotBSevaporation",
    "root_depth",
    "Acttranspiration",
    "saturation",
    "residual",
    "field_capacity",
    "wilting_point",
    "precipitation",
    "OpenWevaporation",
    "ActBSevaporation",
    "PotET",
    "SM"
    
};


static const char *input_var_names[INPUT_VAR_NAME_COUNT] =  {
    "soil_moisture",
    "evaporation",
    "root_depth",
    "transpiration",
    "saturation",
    "residual",
    "field_capacity",
    "wilting_point",
    "hydraulic_conductivity"
};

static int
Get_component_name (Bmi *self, char * name)
{
    strncpy(name, "WOFOST",BMI_MAX_COMPONENT_NAME);
    return BMI_SUCCESS;
}

static int
Get_input_item_count (Bmi *self, int * count)
{
    *count = INPUT_VAR_NAME_COUNT;
    return BMI_SUCCESS;
}


static int
Get_input_var_names(Bmi *self, char ** names)
{
    int i;
    for (i = 0; i< INPUT_VAR_NAME_COUNT; i++) {
        strncpy (names[i], input_var_names[i], BMI_MAX_VAR_NAME);
    }
    
    return BMI_SUCCESS;
}

static int 
Get_output_item_count (Bmi *self, int * count)
{
    *count = OUTPUT_VAR_NAME_COUNT;
    return BMI_SUCCESS;
}

static int 
Get_output_var_names (Bmi *self, char ** names)
{
    int i;
    for (i = 0; i< OUTPUT_VAR_NAME_COUNT; i++) {
        strncpy (names[i], output_var_names[i], BMI_MAX_VAR_NAME);
    }
    
    return BMI_SUCCESS;
}


//Initialize
static int 
Initialize (Bmi *self, const char *file)
{    
    if(!self){
        return BMI_FAILURE;
    }
    fprintf(stderr,"%s \n ", "Initializing .."); 
    if (file){
        fprintf(stderr,"%s \n ", "startInitializing .."); 
        wofost_initialize_config(file);
        fprintf(stderr,"%s \n ", "endInitializing ..");
        self->data = Grids;
    }

    return BMI_SUCCESS;
}

int dag = 0;
//Update
static int 
Update (Bmi *self)
{
	Day = dag;
	
    wofost_update();
    dag++;   
    return BMI_SUCCESS;
}

//Finalize
static int
Finalize (Bmi *self)
{
    
    wofost_finalize();    
   
    return BMI_SUCCESS;        
}

static int
Get_start_time (Bmi *self, double * time)
{
    *time = 0;
    
    return BMI_SUCCESS;
}

static int
Get_end_time (Bmi *self, double * time)
{
    *time = Meteo->ntime;    
    //fprintf ("end time  %d\n", time);
    return BMI_SUCCESS;
}

static int 
Get_value_ptr (Bmi *self, const char *name, void**dest )
{   
    int status = BMI_FAILURE;
     
    {  
        double *src = NULL;
        if (strcmp (name, "evaporation")==0){
            //src = Grids[Lat][Lon]->crp->st.RootDepth;
            
        }
        if (strcmp (name, "root_depth")==0) {
            //src = Grids[Lat][Lon]->crp->st.RootDepth;
        }  
        *dest = src;
        
        if(src)     
            status = BMI_SUCCESS;
            
        
    } 
    return BMI_SUCCESS; 
}   



static int 
Get_grid_size(Bmi *self, int grid, int * size)
{
    if(grid ==0){
      *size = Meteo->nlon * Meteo->nlat;
      return BMI_SUCCESS;
    }
    else {
      *size =-1;
      return BMI_FAILURE;
    }
}



static int 
Get_var_grid (Bmi *self, const char *name, int * grid)
{
    if (strcmp(name, "evaporation") == 0) {
      *grid = 0;
      return BMI_SUCCESS;
      
    }
    else if (strcmp(name, "root_depth") == 0) {
      *grid = 0;
      return BMI_SUCCESS;
      
    }
    else {
    *grid = -1 ;
    return BMI_FAILURE;
    
    }

}

static int
Get_var_nbytes (Bmi *self, const char *name, int * nbytes)
{
    
    int status = BMI_FAILURE;
    {
      int size = 0;
      int grid;
      
      *nbytes = -1;
      if (Get_var_grid(self,name,&grid) == BMI_FAILURE)
        return BMI_FAILURE;
        
      if (Get_grid_size(self, grid, &size) == BMI_FAILURE)
        return BMI_FAILURE;
        
      *nbytes = sizeof (float) * size;
       status = BMI_SUCCESS;
     }
    
       return status;
}

static int
Get_value (Bmi *self, const char *name, void * dest)
{
    if (strcmp(name, "root_depth") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = Grids[Lat][Lon]->crp->st.RootDepth ;
				//fprintf(stderr, "%f\n",dest_float[cpt++] );
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "ActBSevaporation") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->soil->rt.EvapSoil)*0.01;
				//cpt;
				//fprintf(stderr, "%f\n",dest_float[cpt++] );
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "Acttranspiration") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->soil->rt.Transpiration )*0.01;
				
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "PotBSevaporation") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->crp->OWE)*0.01 ;
				//fprintf(stderr, "%f\n",dest_float[cpt++] );
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "PotET") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->crp->ET) *0.01;
				
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "OpenWevaporation") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->soil->rt.EvapWater) *0.01;
				
			}
		}
		  
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "SM") == 0) {
		double* dest_float = (double*)dest;
		
		unsigned cpt = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{
				dest_float[cpt++] = (Grids[Lat][Lon]->soil->st.Moisture);
				
			}
		}
		  
		return BMI_SUCCESS;
	}
	else {
			return BMI_FAILURE;
	}
}


static int
Set_value(Bmi *self, const char *name, void *array)
{
    
    if (strcmp(name, "soil_moisture") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->se_hat = array_float[k] ;	
				k++;
				//fprintf(stderr, "%f\n", Grids[Lat][Lon]->se_hat);
			}	
		} 	
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "saturation") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->soil->ct.MoistureSAT = array_float[k] ;	
				k++;
			}	
		} 	
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "residual") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->theta_res = array_float[k] ;	
				k++;
			}	
		}
		return BMI_SUCCESS;
	}	 
	else if (strcmp(name, "field_capacity") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->soil->ct.MoistureFC = array_float[k] ;	
				k++;
			}	
		} 	
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "wilting_point") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->soil->ct.MoistureWP = array_float[k] ;	
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "hydraulic_conductivity") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{		
				Grids[Lat][Lon]->soil->ct.K0 = array_float[k] ;	
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "precipitation") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Rain[0][Lat][Lon] = array_float[k] ;	
					//fprintf(stderr, "%f \n", Rain[0][Lat][Lon]);
					Rain[0][Lat][Lon] = roundz(100 * Rain[0][Lat][Lon], 2);
					
				} else {
						
					Rain[0][Lat][Lon] = -99;
					//fprintf(stderr, "%f \n", Rain[0][Lat][Lon]);
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "temperature_max") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Tmax[0][Lat][Lon] = array_float[k] ;	
					//fprintf(stderr, "%f \n", Tmax[0][Lat][Lon]);
					Tmax[0][Lat][Lon] = roundz(Tmax[0][Lat][Lon], 1);
					
				} else {
						
					Tmax[0][Lat][Lon] = -99;
					//fprintf(stderr, "%f \n", Tmax[0][Lat][Lon]);
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "temperature_min") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Tmin[0][Lat][Lon] = array_float[k] ;	
					Tmin[0][Lat][Lon] = roundz(Tmin[0][Lat][Lon], 1);
					
				} else {
						
					Tmin[0][Lat][Lon] = -99;
					
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "wind_speed") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Windspeed[0][Lat][Lon] = array_float[k] ;	
					//fprintf(stderr, "%f \n", Windspeed[0][Lat][Lon]);
					Windspeed[0][Lat][Lon] = roundz(Windspeed[0][Lat][Lon], 1);
					
				} else {
						
					Windspeed[0][Lat][Lon] = -99;
					//fprintf(stderr, "%f \n", Windspeed[0][Lat][Lon]);
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "shortwave_radiation") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Radiation[0][Lat][Lon] = array_float[k] ;	
					//fprintf(stderr, "%f \n", Radiation[0][Lat][Lon]);
					Radiation[0][Lat][Lon] = 1000 * roundz(86.400 * Radiation[0][Lat][Lon], 1); // from W/m2 to J/m2/day
					
				} else {
						
					Radiation[0][Lat][Lon] = -99;
					//fprintf(stderr, "%f \n", Radiation[0][Lat][Lon]);
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}
	else if (strcmp(name, "vapour_pressure") == 0) {
        float* array_float = (float*)array;
		int k = 0;
		for (Lat = 0; Lat < Meteo->nlat; Lat++) 
		{
			for(Lon = 0; Lon < Meteo->nlon ; Lon++) 

			{	
				if (Mask[Lat][Lon] >= 1) {
					
					Vapour[0][Lat][Lon] = array_float[k] ;	
					//fprintf(stderr, "%f \n", Vapour[0][Lat][Lon]);
					Vapour[0][Lat][Lon] = roundz(0.01 * Vapour[0][Lat][Lon], 1);
					
				} else {
						
					Vapour[0][Lat][Lon] = -99;
					//fprintf(stderr, "%f \n", Vapour[0][Lat][Lon]);
				}
				
				
				k++;
			}	
		} 		
		return BMI_SUCCESS;
	}    
}


SimUnit * 
new_bmi_wofost(void)
{
    SimUnit *data;
    
    data = (SimUnit*) malloc(sizeof(SimUnit));
    return data;
    
}


Bmi*
register_bmi_wofost(Bmi *model)
{
  if (model) {
      
  model->data = (void*)new_bmi_wofost;
  
  model->initialize = Initialize;
  model->update = Update;
  model->finalize = Finalize;
  
  model->get_start_time = Get_start_time;
  model->get_end_time = Get_end_time;
  
  model->get_component_name = Get_component_name;
  model->get_input_item_count = Get_input_item_count;
  model->get_output_item_count = Get_output_item_count;
  model->get_input_var_names = Get_input_var_names;
  model->get_output_var_names = Get_output_var_names;
  
  model->get_value = Get_value;
  model->set_value = Set_value;
  
  model->get_value_ptr = Get_value_ptr;
  model->get_var_nbytes = Get_var_nbytes;
  model->get_var_grid = Get_var_grid;
  model->get_grid_size = Get_grid_size;
  }  
    
  return model;     
    
}


FILE **output;

//Supporting functions

void wofost_initialize_config(const char * filename)
{
    
    Weather* head = NULL;
    
    SimUnit* initial = NULL;
    char name[MAX_STRING];
    int NumberOfFiles;
    
    NumberOfFiles = GetMeteoInput(filename);
    //fprintf(stderr,"%d \n", NumberOfFiles);

    /* Set the initial Grid address */
    initial = Grid;
    //fprintf(stderr,"%p \n",initial);
    
    /* Set the intial Meteo adress */
    head = Meteo;
    
    /* Allocate memory for the file pointers */
    output = malloc(sizeof(**output) * NumberOfFiles);
    memset(output,0,sizeof(**output) *NumberOfFiles);
    
    /* Go back to the beginning of the list */
    Grid = initial;
    
    /* Open the output files */
    while (Grid)
    {   /* Make valgrind happy  */
        memset(name,'\0',MAX_STRING);
        strncpy(name, Grid->output,strlen(Grid->output));
        output[Grid->file] = fopen("/eejit/home/chevu001/PCR-WOFOST/TW/Output/Maize.txt", "w"); 
        //output[Grid->file] = fopen(name, "w");
        if(output[Grid->file] == NULL){
            fprintf(stderr, "Cannot initialize output file %s.\n", name);
            exit(0);
        }

        header(output[Grid->file]);
        
        Grid = Grid->next;
    }

    /* Go back to the beginning of the list */
    Grid = initial;
    
    Meteo = head;
    if(GetMeteoData(Meteo) != 1) {
		fprintf(stderr, "Cannot get meteo data.\n");
        exit(0);
    }
   
    //fprintf(stderr,"%p \n",Grid);
    //fprintf(stderr,"%p \n",Meteo);
    /* Copy simdata */
    if(GetSimData(Meteo,Grid) != 1) {
        fprintf(stderr, "Cannot copy sim data.\n");
        exit(0);
    }
}


int wofost_update()
{
    
    printf("running %d - %d\n", Meteo->StartYear, Meteo->EndYear);
    for (Lat = 0; Lat < Meteo->nlat ; Lat++) 
	{
		for(Lon = 0 ; Lon < Meteo->nlon ; Lon++) 
		{
            fprintf(stderr, "Location: %lf N - %lf E\n", Latitude[Lat], Longitude[Lon]);
            fprintf(stderr, "Location: %ld - %ld \n", Lat, Lon);
            Grid = Grids[Lat][Lon];

            fprintf(stderr,"%p \n",Grid);
			
            /* Set the date struct */
            memset(&current_date, 0, sizeof(current_date)); 
            current_date.tm_year = MeteoYear[Day] -1900;
            current_date.tm_mday =  0 + MeteoDay[Day];
            mktime(&current_date);
            printf("%4ld %4d \n", Day, MeteoYear[Day]);
            
            /* Storing tmin data*/
                        
            Tlow[Day][Lat][Lon] = Tmin[0][Lat][Lon];
            
             /* Only simulate between start and end year */
            if ( MeteoYear[Day] >=  Meteo->StartYear && MeteoYear[Day] <= Meteo->EndYear)
            {

                run(Grid);
            }

        }
    }
    
    return 1;
    
}

int Step =1;

void run (SimUnit* Grid)
{
    int Emergence;
    int CycleLength   = 300;
    //Step = step;
    while (Grid)
    {
        /* Get data, states and rates from the Grid structure and */
        /* put them in the place holders */
        Crop      = Grid->crp;
        WatBal    = Grid->soil;
        Mng       = Grid->mng;
        Site      = Grid->ste;

        //Start     = Grid->start;
        Emergence = Grid->emergence; /* Start simulation at sowing or emergence */
		
        Temp = 0.5 * (Tmax[0][Lat][Lon] + Tmin[0][Lat][Lon]);
        DayTemp = 0.5 * (Tmax[0][Lat][Lon] + Temp);
		
		
        /* Determine if the sowing already has occurred */
        IfSowing(Grid->start);
        
		WatBal->st.Moisture = Grids[Lat][Lon]->theta_res + ((Grids[Lat][Lon]->soil->ct.MoistureSAT - Grids[Lat][Lon]->theta_res)*Grids[Lat][Lon]->se_hat); 
			
		
        /* If sowing has occurred than determine the emergence */
        if (Crop->Sowing >= 1 && Crop->Emergence == 0)
        {
            if (EmergenceCrop(Emergence))
            {                 
                /* Initializeset state variables */
                InitializeCrop();
                InitializeWatBal();
                InitializeNutrients(); 
            }  
        }

        if (Crop->Sowing >= 1 && Crop->Emergence == 1)
        {   
            if (Crop->st.Development <= (Crop->prm.DevelopStageHarvest) && Crop->GrowthDay < CycleLength) 
            {
                Astro();
                CalcPenman();
                CalcPenmanMonteith();
				
               /* Calculate the evapotranspiration */
                EvapTra();

                /* Set the rate variables to zero */
                RatesToZero();
                

                 /* Rate calculations */
                RateCalulationWatBal();
                Partioning();
                RateCalcultionNutrients();
                RateCalculationCrop();


                /* Calculate LAI */
                Crop->st.LAI = LeaveAreaIndex();             

                /* State calculations */
                IntegrationCrop();
                IntegrationWatBal();
                IntegrationNutrients();
				
				//Cummulative potential evaporation
                Crop->ESO = Crop->ESO + Evtra.MaxEvapSoil;
                //daily rate of potential evaporation               
                Crop->OWE = Evtra.MaxEvapSoil;
                
                //Cummulative potential transpiration
                Crop->POT = Crop->POT + Evtra.MaxTranspiration;             
                //daily rate of actual transpiration
                Crop->AESO = WatBal->rt.EvapSoil;
                //Cummulative actual transpiration
                Crop->ATRA = Crop->ATRA + WatBal->rt.Transpiration; 
                
                //Potential evapotranspiration
                Crop->ET  = Evtra.MaxEvapSoil + Evtra.MaxTranspiration;
                
                
				/* Update the number of days that the crop has grown*/
                Crop->GrowthDay++;

                /* Write to the output files */
                //Output(output[Grid->file]); 
                /*fprintf(stderr,"file ptr: %p",output[Grid->file]);

                fprintf(stderr,"%s : %d",__FILE__, __LINE__);*/
                fprintf(stderr,"%s","Develpement");
                fprintf(stderr,"%f\n",Crop->st.Development);
                fprintf(stderr,"%f\n",Crop->st.RootDepth);
                fprintf(stderr,"%f\n",Crop->st.Development);
                fprintf(stderr,"%f\n",WatBal->st.Moisture);
                
                
            }
            else
            {
                /* Write to the output files */
				fprintf(stderr,"%s","Check");
				fprintf(stderr,"%f",Crop->st.RootDepth);
                Output(output[Grid->file]);
                /*fprintf(stderr,"%s : %d",__FILE__, __LINE__);*/
                Emergence = 0;
                Crop->TSumEmergence = 0;
                Crop->Emergence = 0;
                Crop->Sowing    = 0;
                Crop->st.RootDepth = 0;
				Crop->OWE = 1e20;
				Crop->ET = 1e20;
				WatBal->rt.EvapSoil = 1e20;
				WatBal->rt.EvapWater = 1e20 ;
				WatBal->rt.Transpiration = 1e20;
				//exit(1);
				
            }


        }
      /* Store the daily calculations in the Grid structure */

        Grid->crp  = Crop;
        Grid->soil = WatBal;
        Grid->mng  = Mng;
        Grid->ste  = Site;
        Grid = Grid->next;
    }
}


int wofost_finalize()
{
    SimUnit* initial = NULL;
    Weather* head = NULL;
    
    /* Set the initial Grid address */
    head = Meteo;
    Grid = Grids[0][0];
    initial = Grid;
    //fprintf(stderr,"%p \n",head);
    //fprintf(stderr,"%p \n",initial);
    
   /* Go back to the beginning of the list */ 
    while (Grid)
    {
        fclose(output[Grid->file]);
        Grid = Grid->next;
    }
    free(output);
    
    
    for (Lat = 0; Lat < Meteo->nlat ; Lat++) 
	{
		for(Lon = 0; Lon < Meteo->nlon; Lon++) 
		{
            Grid = Grids[Lat][Lon];
            while(Grid)
            {
                initial = Grid;
                Grid = Grid->next;

                free(initial);
            }
            Clean(Grid);
           
        }
    }
     
    //fprintf(stderr,"%p \n",Grid);
       
    /* Go back to the beginning of the list */
    Meteo = head;
    //fprintf(stderr,"%p \n",Meteo);
    while(Meteo) {
        head = Meteo;
        Meteo = Meteo->next;
        //CleanMeteo(head);
        free(head);
    }
    free(Meteo);
    //fprintf(stderr,"%p \n",Meteo);
    
    return 1;
}



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
#include "wofost.h"
#include "extern.h"
#include "penman.h"

#include "bmi.h"
#include "wofost.h"
#include "wofost_bmi.h"


int main(int argc, char *argv[]) 
{
    Bmi *model =(Bmi *) malloc(sizeof(Bmi));
    
    if (argc != 2) {
        printf("Usage: run_bmiwofostc CONFIGURATION_FILE\n\n");
        printf("Run the wofostc model through its BMI with a configuration file.\n");
        printf("Output is written to the file `bmiwofostc.out`.\n");
        return BMI_SUCCESS;
    }    
    
    FILE *fp = fopen("bmiwofostc.out","w");
    fprintf(fp, "configuration file = %s\n", argv[1]);
    
    
    register_bmi_wofost(model);
    //Initialize
    fprintf(fp, "Initializing... ");
    model->initialize(model, argv[1]);
    fprintf(fp, "done\n");
    
    //Update
    for (Day = 0; Day < Meteo->ntime; Day ++)
    {
        model->update(model);
    }
    
    //Finalize
    fprintf(fp, "Finalizing ..");
    model->finalize(model);
    fprintf(fp, "done\n");
    
    free(model);
    fclose(fp);        

    return (EXIT_SUCCESS);
}




























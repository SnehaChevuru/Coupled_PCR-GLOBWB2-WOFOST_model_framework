#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "soil.h"


void GetSoilData(Soil *SOIL, char *soilfile)
{
    TABLE *Table[NR_TABLES_SOIL], *start;

    char line[MAX_STRING];
    int i, c, count;
    float Variable[100], XValue, YValue;
    char x[2], xx[2],  word[100];
    FILE *fq;

    if ((fq = fopen(soilfile, "rt")) == NULL)
    {
        fprintf(stderr, "Cannot open input file %s.\n", soilfile); 
        exit(0);
    }

    i=0;
    count = 0;
    while (strcmp(SoilParam[i],"NULL")) 
    {
        while ((c=fscanf(fq,"%s",word)) != EOF && i < 12 ) 
        {
            if (strlen(word)> 98) 
            {
                fprintf(stderr, "Check the site input file: very long strings.\n"); 
                exit(0);
            }
            if (!strcmp(word, SoilParam[i])) {
                while ((c=fgetc(fq)) !='=');
                fscanf(fq,"%f",  &Variable[i]);
                count++;
                break;
            }
        }
        rewind(fq);
        i++;
    }

    if (i != NR_VARIABLES_SOIL) 
    {
       fprintf(stderr, "Something wrong with the Soil variables in file %s.\n", soilfile);
       exit(0);
    }
 
    rewind(fq);  

    FillSoilVariables(SOIL, Variable);
 

    i=0;
    count = 0;
    while (strcmp(SoilParam2[i],"NULL")) {
        while(fgets(line, MAX_STRING, fq)) {
            if(line[0] == '*' || line[0] == ' ' || line[0] == '\n' || line[0] == '\r'){
                continue;
            }

            sscanf(line,"%s",word);
            if (!strcmp(word, SoilParam2[i])) {

                c = sscanf(line,"%s %s %f %s  %f", word, x, &XValue, xx, &YValue);

                Table[i]= start= malloc(sizeof(TABLE));
                Table[i]->next = NULL;      
                Table[i]->x = XValue;
                Table[i]->y = YValue;            

                while (fgets(line, MAX_STRING, fq)) {  
                    if((c = sscanf(line," %f %s  %f",  &XValue, xx, &YValue)) != 3) break;

                    Table[i]->next = malloc(sizeof(TABLE));
                    Table[i] = Table[i]->next; 
                    Table[i]->next = NULL;
                    Table[i]->x = XValue;
                    Table[i]->y = YValue;
                }
                /* Go back to beginning of the table */
                Table[i] = start;
                count++;
                break;
            }
        } 
        rewind(fq);    
        i++; 
    }

    fclose(fq);

    if (count!= NR_TABLES_SOIL) 
    {
       fprintf(stderr, "Something wrong with the Soil tables in file %s.\n", soilfile);
       exit(0);
    }
 
    SOIL->VolumetricSoilMoisture = Table[0];
    SOIL->HydraulicConductivity  = Table[1];
  
    /* Set state variables of the water balance are set to zero. */   
    SOIL->st.EvapWater         = 0.;
    SOIL->st.EvapSoil          = 0.;
    SOIL->st.Infiltration      = 0.;
    SOIL->st.Irrigation        = 0.;
    SOIL->st.Loss              = 0.;
    SOIL->st.Moisture          = 0.;
    SOIL->st.MoistureLOW       = 0.;
    SOIL->st.Percolation       = 0.;
    SOIL->st.Rain              = 0.;
    SOIL->st.RootZoneMoisture  = 0.;
    SOIL->st.Runoff            = 0.;
    SOIL->st.SurfaceStorage    = 0.;
    SOIL->st.Transpiration     = 0.;
    SOIL->st.WaterRootExt      = 0.;
	SOIL->rt.EvapWater = 1e20;
	SOIL->rt.EvapSoil = 1e20;
	SOIL->rt.Transpiration = 1e20;
}

/*void FluxesToZero()
{
	//Crop->st.RootDepth = 0.;
	CROP->OWE = 0.;
	CROP->ET = 0.;
	SOIL->rt.EvapSoil = 0.;
	SOIL->rt.EvapWater = 0. ;
	SOIL->rt.Transpiration = 0.;
		
}*/

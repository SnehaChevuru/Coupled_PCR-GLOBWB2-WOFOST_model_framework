#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "crop.h"
#include "extern.h"

/* ------------------------------------------------------------------------*/
/*  function GetCropData()                                                 */
/*  Purpose: Read the Wofost crop file and store the parameters and tables */
/* ------------------------------------------------------------------------*/

void GetCropData(Plant *CROP, char *cropfile)
{
    TABLE *Table[NR_TABLES_CRP], *start;

    char line[MAX_STRING];
    int i, c, count;
    float Variable[NR_VARIABLES_CRP], XValue, YValue;
    char x[2], xx[2],  word[100];
    FILE *fq;


    if ((fq = fopen(cropfile, "rt")) == NULL)
    {
        fprintf(stderr, "Cannot open input file %s.\n", cropfile); 
        exit(0);
    }

    i=0;
    count = 0;
    while (strcmp(CropParam[i],"NULL")) 
    {
        while ((c=fscanf(fq,"%s",word)) != EOF )
        {
            if (strlen(word)> 98) 
            {
                fprintf(stderr, "Check the site input file: very long strings.\n"); 
                exit(0);
            }
            if (!strcmp(word, CropParam[i])) 
            {
                while ((c=fgetc(fq)) !='=');
                fscanf(fq,"%f",  &Variable[i]);
                count++;
                break;
            }
        }  
        rewind(fq);
        i++;
    }


    if  (count == NR_VARIABLES_CRP  || count == NR_VARIABLES_CRP - 2)
        ;
    else
    {
       fprintf(stderr, "Something wrong with the Crop variables in file %s.\n", cropfile);
       exit(0);
    }

    rewind(fq);  
 
    FillCropVariables(CROP, Variable);

    i = 0;
    count = 0;
    while (strcmp(CropParam2[i],"NULL")) {
        while(fgets(line, MAX_STRING, fq)) {
            if(line[0] == '*' || line[0] == ' ' || line[0] == '\n' || line[0] == '\r'){
                continue;
            }
            
            sscanf(line,"%s",word);
            if (!strcmp(word, CropParam2[i])) {
                
                c = sscanf(line,"%s %s %f %s  %f", word, x, &XValue, xx, &YValue);
                
                Table[i] = start= malloc(sizeof(TABLE));
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
  
    if (count == NR_TABLES_CRP || count == NR_TABLES_CRP - 1)
        ;
    else
    {
        fprintf(stderr, "Something wrong with the Crop tables in file %s.\n", cropfile); 
        exit(0);
    } 

    if (CROP->prm.IdentifyAnthesis < 2)
    {
       CROP->prm.VernalizationRate    = NULL;
    }
    else
    {
        CROP->prm.VernalizationRate    = Table[0];
    } 

    CROP->prm.DeltaTempSum         = Table[1];
    CROP->prm.SpecificLeaveArea    = Table[2];
    CROP->prm.SpecificStemArea     = Table[3];
    CROP->prm.KDiffuseTb           = Table[4];
    CROP->prm.EFFTb                = Table[5];
    CROP->prm.MaxAssimRate         = Table[6];
    CROP->prm.FactorAssimRateTemp  = Table[7];
    CROP->prm.FactorGrossAssimTemp = Table[8];
    CROP->prm.CO2AMAXTB            = Table[9];
    CROP->prm.CO2EFFTB             = Table[10];
    CROP->prm.CO2TRATB             = Table[11];
    CROP->prm.FactorSenescence     = Table[12];
    CROP->prm.Roots                = Table[13];
    CROP->prm.Leaves               = Table[14];
    CROP->prm.Stems                = Table[15];
    CROP->prm.Storage              = Table[16];
    CROP->prm.DeathRateStems       = Table[17];
    CROP->prm.DeathRateRoots       = Table[18]; 
    CROP->prm.N_MaxLeaves          = Table[19];
    CROP->prm.P_MaxLeaves          = Table[20];
    CROP->prm.K_MaxLeaves          = Table[21];

    CROP->Emergence = 0;
    CROP->TSumEmergence = 0.;

    /* Crop development has not started yet*/
    CROP->st.RootDepth = 0.;
    CROP->rt.RootDepth = 0.;
    CROP->st.RootDepth_prev = 0.;
    CROP->st.Development = 0.;
    CROP->DaysOxygenStress = 0; // No crop development therefore no oxygen stress

    /* No initial nutrient stress */
    CROP->NutrientStress = 1.;
    CROP->NPK_Indx =1;


    /* STATES */  
    /* Set the initial growth states to zero */
    CROP->st.roots   = 0.;
    CROP->st.stems   = 0.;
    CROP->st.leaves  = 0.;
    CROP->st.storage = 0.;
    CROP->st.LAIExp  = 0.;
    CROP->st.vernalization = 0.;
    
    /*Set the initial dying state to zero */
    CROP->dst.leaves = 0.;
    CROP->dst.stems  = 0.;
    CROP->dst.roots  = 0.;

    /* Set the initial nutrient states to zero*/
    CROP->N_st.leaves = 0.;
    CROP->N_st.stems  = 0.;
    CROP->N_st.roots  = 0.;
    CROP->N_st.storage= 0.;

    CROP->P_st.leaves = 0.;
    CROP->P_st.stems  = 0.;
    CROP->P_st.roots  = 0.;
    CROP->P_st.storage= 0.;

    CROP->K_st.leaves = 0.;
    CROP->K_st.stems  = 0.;
    CROP->K_st.roots  = 0.;
    CROP->K_st.storage= 0.;

    /* Set the maximum nutrient concentration to zero at initialization */
    CROP->N_st.Max_lv = 0.;
    CROP->N_st.Max_st = 0.;
    CROP->N_st.Max_ro = 0.;

    CROP->P_st.Max_lv = 0.;
    CROP->P_st.Max_st = 0.;
    CROP->P_st.Max_ro = 0.;

    CROP->K_st.Max_lv = 0.;
    CROP->K_st.Max_st = 0.;
    CROP->K_st.Max_ro = 0.;

    /* Set the initial optimal leave concentrations to zero */
    CROP->N_st.Optimum_lv = 0;
    CROP->N_st.Optimum_st = 0;

    CROP->P_st.Optimum_lv = 0;
    CROP->P_st.Optimum_st = 0;

    CROP->K_st.Optimum_lv = 0;
    CROP->K_st.Optimum_st = 0;

    /* No nutrient stress at initialization */
    CROP->NPK_Indx  = 1.;
    CROP->N_st.Indx = 1.;
    CROP->P_st.Indx = 1.;
    CROP->K_st.Indx = 1.;

     /* Set the initial uptake states to zero*/
    CROP->N_st.Uptake    = 0.;
    CROP->N_st.Uptake_lv = 0.;
    CROP->N_st.Uptake_st = 0.;
    CROP->N_st.Uptake_ro = 0.;

    CROP->P_st.Uptake    = 0.;
    CROP->P_st.Uptake_lv = 0.;
    CROP->P_st.Uptake_st = 0.;
    CROP->P_st.Uptake_ro = 0.;

    CROP->K_st.Uptake    = 0.;
    CROP->K_st.Uptake_lv = 0.;
    CROP->K_st.Uptake_st = 0.;
    CROP->K_st.Uptake_ro = 0.;

    /* No nutrient losses at initialization */
    CROP->N_st.death_lv = 0.;
    CROP->N_st.death_st = 0.;
    CROP->N_st.death_ro = 0.;

    CROP->P_st.death_lv = 0.;
    CROP->P_st.death_st = 0.;
    CROP->P_st.death_ro = 0.;

    CROP->K_st.death_lv = 0.;
    CROP->K_st.death_st = 0.;
    CROP->K_st.death_ro = 0.;
    
    CROP->OWE = 1e20;
	CROP->ET = 1e20;
    
}


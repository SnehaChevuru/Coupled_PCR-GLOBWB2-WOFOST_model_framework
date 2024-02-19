#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------*/
/*  function SoilNutrientRates()                                  */
/*  Purpose: Calculation of the soil nutrient rates kg ha-1 d-1   */
/* ---------------------------------------------------------------*/     

void SoilNutrientRates()
{
    float N_fert;
    float P_fert;
    float K_fert;
    
    if (Crop->st.Development > 0. && Crop->st.Development <= Crop->prm.DevelopmentStageNLimit)
    {   /* NPK rates that come available through mineralization, cannot exceed */
        /* the available NPK for mineralization                                */
        Site->rt_N_mins = min(Mng->N_Mins * Mng->NRecoveryFrac, Site->st_N_mins); 
        Site->rt_P_mins = min(Mng->P_Mins * Mng->PRecoveryFrac, Site->st_P_mins); 
        Site->rt_K_mins = min(Mng->K_Mins * Mng->KRecoveryFrac, Site->st_K_mins); 
    }
    else
    {
        Site->rt_N_mins = 0.; 
        Site->rt_P_mins = 0.; 
        Site->rt_K_mins = 0.;         
    }
    
    /* NPK amount that comes available for the crop at day_fl through fertilizer applications */
    N_fert = List(Mng->N_Fert_table) * Mng->N_Uptake_frac;
    P_fert = List(Mng->P_Fert_table) * Mng->P_Uptake_frac;
    K_fert = List(Mng->K_Fert_table) * Mng->K_Uptake_frac;
    
    /* Change in total inorganic NPK in soil as function of fertilizer input, */
    /* soil NPK mineralization and crop uptake                                */
    Site->rt_N_tot = (N_fert / Step) - Crop->N_rt.Uptake  + Site->rt_N_mins;
    Site->rt_P_tot = (P_fert / Step) - Crop->P_rt.Uptake  + Site->rt_P_mins;
    Site->rt_K_tot = (K_fert / Step) - Crop->K_rt.Uptake  + Site->rt_K_mins;

}

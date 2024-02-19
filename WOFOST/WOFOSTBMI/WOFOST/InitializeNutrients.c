#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* --------------------------------------------------------------------*/
/*  function NutrientsInitialize()                                     */
/*  Purpose: Initialization of nutrient parameters                     */
/* --------------------------------------------------------------------*/

Management *Mng;

void InitializeNutrients()
{    
    /* Initial maximum N concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop->N_st.Max_lv = Afgen(Crop->prm.N_MaxLeaves, &(Crop->st.Development));
    Crop->N_st.Max_st = Crop->prm.N_MaxStems * Crop->N_st.Max_lv;
    Crop->N_st.Max_ro = Crop->prm.N_MaxRoots * Crop->N_st.Max_lv;
    Crop->N_st.Max_so = 0.;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop->N_st.leaves = Crop->N_st.Max_lv * Crop->st.leaves;
    Crop->N_st.stems  = Crop->N_st.Max_st * Crop->st.stems;
    Crop->N_st.roots  = Crop->N_st.Max_ro * Crop->st.roots;
    Crop->N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop->P_st.Max_lv = Afgen(Crop->prm.P_MaxLeaves, &(Crop->st.Development));
    Crop->P_st.Max_st  = Crop->prm.P_MaxStems * Crop->P_st.Max_lv;
    Crop->P_st.Max_ro  = Crop->prm.P_MaxRoots * Crop->P_st.Max_lv;
    Crop->P_st.Max_so = 0.;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop->P_st.leaves = Crop->P_st.Max_lv * Crop->st.leaves;
    Crop->P_st.stems  = Crop->P_st.Max_st * Crop->st.stems;
    Crop->P_st.roots  = Crop->P_st.Max_ro * Crop->st.roots;
    Crop->P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    Crop->K_st.Max_lv = Afgen(Crop->prm.K_MaxLeaves, &(Crop->st.Development));
    Crop->K_st.Max_st  = Crop->prm.K_MaxStems * Crop->K_st.Max_lv;
    Crop->K_st.Max_ro  = Crop->prm.K_MaxRoots * Crop->K_st.Max_lv;
    Crop->K_st.Max_so = 0.;
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    Crop->K_st.leaves = Crop->K_st.Max_lv * Crop->st.leaves;
    Crop->K_st.stems  = Crop->K_st.Max_st * Crop->st.stems;
    Crop->K_st.roots  = Crop->K_st.Max_ro * Crop->st.roots;
    Crop->K_st.storage = 0.;
    
    /* No nutrient losses at initialization */
    Crop->N_st.death_lv = 0.;
    Crop->N_st.death_st = 0.;
    Crop->N_st.death_ro = 0.;
   
    Crop->P_st.death_lv = 0.;
    Crop->P_st.death_st = 0.;
    Crop->P_st.death_ro = 0.;
  
    Crop->K_st.death_lv = 0.;
    Crop->K_st.death_st = 0.;
    Crop->K_st.death_ro = 0.;
       
    /* Set the initial uptake to zero*/
    Crop->N_st.Uptake    = 0.;
    Crop->N_st.Uptake_lv = 0.;
    Crop->N_st.Uptake_st = 0.;
    Crop->N_st.Uptake_ro = 0.;
    
    Crop->P_st.Uptake    = 0.;
    Crop->P_st.Uptake_lv = 0.;
    Crop->P_st.Uptake_st = 0.;
    Crop->P_st.Uptake_ro = 0.;
    
    Crop->K_st.Uptake    = 0.;
    Crop->K_st.Uptake_lv = 0.;
    Crop->K_st.Uptake_st = 0.;
    Crop->K_st.Uptake_ro = 0.;
    
    
     /* Set the soil nutrient states */
    Site->st_N_tot = List(Mng->N_Fert_table) * Mng->N_Uptake_frac;
    Site->st_P_tot = List(Mng->P_Fert_table) * Mng->P_Uptake_frac;
    Site->st_K_tot = List(Mng->K_Fert_table) * Mng->K_Uptake_frac;

    Site->st_N_mins = Mng->N_Mins;
    Site->st_P_mins = Mng->P_Mins;
    Site->st_K_mins = Mng->K_Mins;
       
    /* No nutrient stress at initialization */
    Crop->N_st.Indx = 1.;
    Crop->P_st.Indx = 1.;
    Crop->K_st.Indx = 1.;
    Crop->NPK_Indx  = 1.;
       
    /* Set the initial optimal leave concentrations to zero */
    Crop->N_st.Optimum_lv = 0;
    Crop->N_st.Optimum_st = 0;
        
}     

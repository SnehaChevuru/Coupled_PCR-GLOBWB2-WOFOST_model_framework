#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function Translocation()                                                  */ 
/*  Purpose: To compute the amount of nutrients in the organs that can        */
/*  be translocated kg ha-1                                                   */
/* ---------------------------------------------------------------------------*/
void NutrientTranslocation()                                                                                                       
{                                                                                                                  
    /* N amount available for translocation */                                                                                                              
    Crop->N_st.Avail_lv = max(0.,Crop->N_st.leaves - Crop->st.leaves * Crop->prm.N_ResidualFrac_lv);
    Crop->N_st.Avail_st = max(0.,Crop->N_st.stems  - Crop->st.stems  * Crop->prm.N_ResidualFrac_st);
    Crop->N_st.Avail_ro = max((Crop->N_st.Avail_lv + Crop->N_st.Avail_st) 
            * Crop->prm.FracTranslocRoots, Crop->N_st.roots - Crop->st.roots * Crop->prm.N_ResidualFrac_ro);
    
   
    /* P amount available for translocation */ 
    Crop->P_st.Avail_lv = max (0.,Crop->P_st.leaves - Crop->st.leaves * Crop->prm.P_ResidualFrac_lv);
    Crop->P_st.Avail_st = max (0.,Crop->P_st.stems  - Crop->st.stems  * Crop->prm.P_ResidualFrac_st);
    Crop->P_st.Avail_ro = max ((Crop->P_st.Avail_lv + Crop->P_st.Avail_st) * Crop->prm.FracTranslocRoots, 
            Crop->P_st.roots - Crop->st.roots * Crop->prm.P_ResidualFrac_ro);
    
    
    /* K amount available for translocation */ 
    Crop->K_st.Avail_lv = max(0.,Crop->K_st.leaves - Crop->st.leaves * Crop->prm.K_ResidualFrac_lv);
    Crop->K_st.Avail_st = max(0.,Crop->K_st.stems  - Crop->st.stems  * Crop->prm.K_ResidualFrac_st);
    Crop->K_st.Avail_ro = max((Crop->K_st.Avail_lv + Crop->K_st.Avail_st) * Crop->prm.FracTranslocRoots, 
            Crop->K_st.roots - Crop->st.roots * Crop->prm.K_ResidualFrac_ro);
    
    /* Total available amount of nutrients */
    Crop->N_st.Avail = Crop->N_st.Avail_lv + Crop->N_st.Avail_st + Crop->N_st.Avail_ro;
    Crop->P_st.Avail = Crop->P_st.Avail_lv + Crop->P_st.Avail_st + Crop->P_st.Avail_ro;
    Crop->K_st.Avail = Crop->K_st.Avail_lv + Crop->K_st.Avail_st + Crop->K_st.Avail_ro;
    
    
    /* Total available nutrient amount for translocation */
    if ( Crop->st.Development > Crop->prm.DevelopmentStageNT)
    {
        Crop->N_rt.Supply = (Crop->N_st.Avail_lv + Crop->N_st.Avail_st + Crop->N_st.Avail_ro)/Crop->prm.TCNT;
        Crop->P_rt.Supply = (Crop->P_st.Avail_lv + Crop->P_st.Avail_st + Crop->P_st.Avail_ro)/Crop->prm.TCPT;
        Crop->K_rt.Supply = (Crop->K_st.Avail_lv + Crop->K_st.Avail_st + Crop->K_st.Avail_ro)/Crop->prm.TCKT;        
    }
    else
    {
        Crop->N_rt.Supply = 0.;
        Crop->P_rt.Supply = 0.;
        Crop->K_rt.Supply = 0.;
    }   
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* -----------------------------------------------------------------------------------------*/
/*  function Partioning()                                                                   */
/*  Purpose: Calculate the partioning factors and correct them for nutrient or water stress */ 
/* -----------------------------------------------------------------------------------------*/

void Partioning()
{
    float factor;
    float flv;
       
    if (WatBal->WaterStress < Crop->N_st.Indx)
    {
        factor = max(1., 1./(WatBal->WaterStress + 0.5));
        Crop->fac_ro = min(0.6, Afgen(Crop->prm.Roots, &(Crop->st.Development)) * factor);
        Crop->fac_lv = Afgen(Crop->prm.Leaves, &(Crop->st.Development));
        Crop->fac_st = Afgen(Crop->prm.Stems, &(Crop->st.Development));
        Crop->fac_so = Afgen(Crop->prm.Storage, &(Crop->st.Development));
    }
    else
    {
        flv = Afgen(Crop->prm.Leaves, &(Crop->st.Development));
        factor = exp(-Crop->prm.N_lv_partitioning * ( 1. - Crop->N_st.Indx));
        Crop->fac_lv = flv * factor;
        Crop->fac_ro = Afgen(Crop->prm.Roots, &(Crop->st.Development));
        Crop->fac_st = Afgen(Crop->prm.Stems, &(Crop->st.Development)) + flv - Crop->fac_lv;
        Crop->fac_so = Afgen(Crop->prm.Storage, &(Crop->st.Development));
    }
}   

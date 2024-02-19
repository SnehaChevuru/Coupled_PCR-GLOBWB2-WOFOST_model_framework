#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function Growth(float NewPlantMaterial)                                   */
/*  Purpose: Establish growth rates of the plant organs (kg ha-1 d-1) and     */
/*  rooting depth (cm)                                                        */
/* ---------------------------------------------------------------------------*/

void Growth(float NewPlantMaterial)
{
    float shoots;
    float Translocation;
    float FRTRL;
     
    /* Water stress is more severe as compared to Nitrogen stress and */
    /* partitioning will follow the original assumptions of LINTUL2   */     
        
    FRTRL = 0.;
    Translocation = 0.;
    if (Crop->st.Development >= 1.)
    {
        Translocation = (Crop->st.stems + Crop->dst.stems) * Crop->rt.Development * FRTRL;
    }
    
    Crop->drt.roots = Crop->st.roots * Afgen(Crop->prm.DeathRateRoots, &(Crop->st.Development));
    Crop->rt.roots  = NewPlantMaterial * Crop->fac_ro - Crop->drt.roots;
    
    shoots         = NewPlantMaterial * (1-Crop->fac_ro);
        
    Crop->drt.stems = Crop->st.stems * Afgen(Crop->prm.DeathRateStems, &(Crop->st.Development));    
    Crop->rt.stems  = shoots * Crop->fac_st - Crop->drt.stems - Translocation;
    
    Crop->rt.storage = shoots * Crop->fac_so + Translocation;
    
    /* Dying leave biomass is subtracted from the oldest classes */
    Crop->drt.leaves = DyingLeaves(); 
    Crop->rt.leaves  = shoots * Crop->fac_lv;
    
    /* Note that the new leave biomass is stored in the youngest leave class */
    LeaveGrowth();
    
    Crop->rt.leaves  = Crop->rt.leaves -  Crop->drt.leaves;
    
    
    /* No Root growth if no assimilates are partitioned to the roots or if */
    /* the crop has no airducts and the roots are close to the groundwater */
    if (Crop->fac_ro <= 0.0 || (!Crop->prm.Airducts && Site->GroundwaterDepth - Crop->st.RootDepth < 10.))
        Crop->rt.RootDepth = 0.;
    else
        Crop->rt.RootDepth = min(Crop->prm.MaxRootingDepth - Crop->st.RootDepth,
                Crop->prm.MaxIncreaseRoot*Step);
}   

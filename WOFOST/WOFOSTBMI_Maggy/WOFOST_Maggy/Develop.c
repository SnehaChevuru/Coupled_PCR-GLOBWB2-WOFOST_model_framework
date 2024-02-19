#include <stdio.h>
#include "astro.h"
#include "extern.h"
#include "wofost.h"

/* ----------------------------------------------------------------------------*/
/*  function GetDevelopmentStage()                                             */
/*  Purpose: Calculation of the development stage as function of the effective */
/*  daily temperature, day length and the vernalization                        */
/*-----------------------------------------------------------------------------*/

float Temp;

void DevelopmentRate(void)
{
    float VernalizationFactor;

    if (Crop->st.Development  < 1.)
    {
        Crop->rt.Development = Afgen(Crop->prm.DeltaTempSum, &Temp)/Crop->prm.TempSum1;
        if (Crop->prm.IdentifyAnthesis == 1 || Crop->prm.IdentifyAnthesis == 2) 
            Crop->rt.Development = Crop->rt.Development *
                limit(0., 1., (PARDaylength - Crop->prm.CriticalDaylength)/(
                    Crop->prm.OptimumDaylength-Crop->prm.CriticalDaylength));
        
        /* Vernalization takes place */
        if (Crop->prm.IdentifyAnthesis == 2)
        {
            Crop->rt.vernalization = insw(Crop->st.Development - 0.3, Afgen(Crop->prm.VernalizationRate,&Temp), 0.);
            VernalizationFactor = limit(0., 1., 
                    (Crop->st.vernalization - Crop->prm.BaseVernRequirement)/
                    (Crop->prm.SatVernRequirement - Crop->prm.BaseVernRequirement));
            
        Crop->rt.Development = Crop->rt.Development * insw(Crop->st.Development - 0.3, VernalizationFactor, 1.);
        }
    }
    else 
        Crop->rt.Development = Afgen(Crop->prm.DeltaTempSum, &Temp)/Crop->prm.TempSum2; 
        fprintf(stderr,"%s","RateDevelpement");
        fprintf(stderr,"%f",Crop->rt.Development); 
}

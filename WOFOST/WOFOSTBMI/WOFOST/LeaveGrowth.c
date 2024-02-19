#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "extern.h"
#include "wofost.h"

/* ---------------------------------------------------------------------------*/
/*  function LeaveGrowth(float LAIExp, float NewLeaves)                       */
/*  Purpose: Calculation of the daily leaves growth rate, the results are     */
/*           stored in the Crop->properties linked list                       */
/* ---------------------------------------------------------------------------*/

void LeaveGrowth()
{
    float GrowthExpLAI;
    float GrowthSourceLimited;
    float SpecLeafArea; 
    float Stress;
    float DTeff;

    Green *Leave = NULL;
    Green *LeaveProperties = NULL;

    /* Specific Leaf area(m2/g), as dependent on NPK stress */
    SpecLeafArea = Afgen(Crop->prm.SpecificLeaveArea, &(Crop->st.Development)) * 
        exp(-Crop->prm.NutrientStessSLA * (1.-Crop->NPK_Indx));

    /* Leave area not to exceed exponential growth */
    if (Crop->st.LAIExp < 6 && Crop->rt.leaves > 0.) 
    {
        /* Growth during juvenile stage */
        if (Crop->st.Development < 0.2 && Crop->st.LAI < 0.75)
        {
            Stress = WatBal->WaterStress * exp(-Crop->prm.NitrogenStressLAI * (1. - Crop->NPK_Indx));
        }
        else
        {
            Stress = 1.;
        }
       
        /* Effective temperature for leave growth */
        DTeff = max(0.,Temp - Crop->prm.TempBaseLeaves);
        
        /* Correction for nutrient stress */
        GrowthExpLAI = Crop->st.LAIExp * Crop->prm.RelIncreaseLAI * DTeff * Stress;

        /* Source limited leaf area increase */
        GrowthSourceLimited = Crop->rt.leaves * SpecLeafArea;

        /* Sink-limited leaf area increase */
        SpecLeafArea = min(GrowthExpLAI, GrowthSourceLimited)/Crop->rt.leaves;
    }
    else
    {
        GrowthExpLAI = 0.;
    }
    
        
    Leave         = malloc(sizeof(Green));
    Leave->weight = Crop->rt.leaves;
    Leave->age    = 0.;
    Leave->area   = SpecLeafArea;
    Leave->next   = NULL;

    /* Store the start address */
    LeaveProperties = Crop->LeaveProperties;

    /* Loop until the last element in the list */
    while (Crop->LeaveProperties->next != NULL)
    {
        Crop->LeaveProperties = Crop->LeaveProperties->next; 
    }

    /* Add new element to the list */
    Crop->LeaveProperties->next = Leave;

    /* Restore the start address */
    Crop->LeaveProperties = LeaveProperties;
    
    Crop->rt.LAIExp = GrowthExpLAI;
}

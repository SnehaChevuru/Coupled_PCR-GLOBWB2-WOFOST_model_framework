#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function IntegrationCrop                                                  */
/*  Purpose: Establish the crop state variables by integration of the crop    */
/*  rate variables and update the crop leave classes                          */
/* ---------------------------------------------------------------------------*/

void IntegrationCrop()      
{
    float PhysAgeing;
    Green *LeaveProperties;
    
    Crop->st.roots    += Crop->rt.roots;
    Crop->st.stems    += Crop->rt.stems;
    Crop->st.leaves   += Crop->rt.leaves;
    Crop->st.storage  += Crop->rt.storage;
    Crop->st.LAIExp   += Crop->rt.LAIExp;
    
    Crop->st.RootDepth_prev = Crop->st.RootDepth;
    Crop->st.RootDepth += Crop->rt.RootDepth;
    
    /* Calculate the developmentstage */
    if (Crop->st.Development < 1.)
    {
        Crop->st.Development += Crop->rt.Development;
        if (Crop->st.Development >1. )
            Crop->st.Development = 1.;
    }
    else
        Crop->st.Development += Crop->rt.Development;
    
    /* Calculate the amount of death material kg ha-1 */
    Crop->dst.roots    += Crop->drt.roots;
    Crop->dst.stems    += Crop->drt.stems;
    Crop->dst.leaves   += Crop->drt.leaves;
    
    /* Calculate vernalization state in case the switch is set */
    if (Crop->prm.IdentifyAnthesis == 2)
    {
        Crop->st.vernalization += Crop->rt.vernalization;       
    }

    /* Establish the age increase */
    PhysAgeing = max(0., (Temp - Crop->prm.TempBaseLeaves)/(35.- Crop->prm.TempBaseLeaves));
    
    /* Store the initial address */
    LeaveProperties = Crop->LeaveProperties;
    
    /* Update the leave age for each age class */
    while (Crop->LeaveProperties->next)
    {
        Crop->LeaveProperties->age += PhysAgeing;
        Crop->LeaveProperties      = Crop->LeaveProperties->next;
    }
  
    /* Return to beginning of the linked list */
    Crop->LeaveProperties = LeaveProperties;
   
}                

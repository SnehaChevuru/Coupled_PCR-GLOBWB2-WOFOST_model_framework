#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ----------------------------------------------------------------------------------*/
/*  function Conversion                                                              */
/*  Purpose: Convert the net assimilation products into plant dry matter kg ha-1 d-1 */
/* ----------------------------------------------------------------------------------*/

float Conversion(float NetAssimilation)
{
    float root, shoots;
    
    root  = Crop->fac_ro/Crop->prm.ConversionRoots;
    shoots =  Crop->fac_st/Crop->prm.ConversionStems;
    shoots += Crop->fac_lv/Crop->prm.ConversionLeaves;  
    shoots += Crop->fac_so/Crop->prm.ConversionStorage;
    
    /* conversion */
    return NetAssimilation/(shoots*(1-Crop->fac_ro)+root);
}

#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "wofost.h"
#include "penman.h"

void header(FILE *fp)
{
    
    fprintf(fp,"Lat\tLon\tYear\tday\tGrowth\tTSUM1\tTSUM2\tTWSO\tLBM\tWS\tSM\tSat\tESO\tTRA\n(-)\t(-)\t(-)\t(day)\t(dk)\t(-)\t(-)\t(kg)\t(kg)\t(kg)\t(kg)\t(kg)\t(mm)\t(mm)\n");
    
}

void Output(FILE *fp)
{
    //float ave, adev, sdev, var, skew, curt;
    //int mnth, dy, dekad;
    float LivingBiomass;
    float TotalBiomass;
    
    //Biomass
    LivingBiomass = Crop->st.leaves + Crop->st.stems + Crop->st.storage;
    TotalBiomass = LivingBiomass + Crop->dst.leaves + Crop->dst.stems;
    
    //printf("%4d\n", MeteoYear[Day]);
    
    /*sscanf(Grid->start,"%2d-%2d",&mnth,&dy);
   
    if (mnth < 1 || mnth > 12) exit(0);
    if (dy < 1 || dy >31) exit (0);
     
    // convert sowing day to dekad
    dekad = (mnth -1) * 3;
    if (dy <= 10)
        dekad += 1;
    else if(dy <=20)
        dekad += 2;
    else
        dekad += 3;  */          
    
    //Moment(Grid->twso, Count, &ave, &adev, &sdev, &var, &skew, &curt);
    //fprintf(stderr,"%s : %d",__FILE__, __LINE__);
    fprintf(fp, "%4.2f\t %4.2f\t %4d\t%4d\t %4d\t %4.0f\t %4.0f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t%5.2f\t %5.2f\n", 
    Latitude[Lat], 
    Longitude[Lon], 
    MeteoYear[Day],
    MeteoDay[Day],
    //Planting_date[Lat][Lon],
    //dekad,   
    Crop->GrowthDay,
    Crop->prm.TempSum1,
    Crop->prm.TempSum2,
    //Crop->st.Development,
    //Crop->st.leaves,
    //Crop->st.stems,
    Crop->st.storage,
    //Crop->dst.leaves,
    //Crop->dst.stems,
    LivingBiomass,
    WatBal->WaterStress,
    //Evtra.MaxEvapSoil,
    WatBal->st.Moisture,
    WatBal->ct.MoistureFC,
    //TotalBiomass,
    10*Crop->ESO,
    10*Crop->ATRA
    );
}


/*
void header(FILE *fp)
{
    //fprintf(fp,"%7.2f\t%7.2f\t\t%4d\t\t%3d\t\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t"
      //     "\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\n");
    fprintf(fp,"Lat,Lon,Year,Day,Deve,leaves,stems,storage,roots,LAI,WS,M,Rain, Infil,runoff,loss,N,P,K_I,N,P<K_T,N,P,K_UP,N,P,K_dem\n");
}    


void Output(FILE *fp)
{   
    fprintf(fp,"%7.2f\t%7.2f\t\t%4d\t\t%3d\t\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t"
           "\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\t%4.2f\n",
    //fprintf(fp,"Lat,Lon,Year,Day,Deve,leaves,stems,storage,roots,LAI,WS,M,Rain, Infil,runoff,loss,N,P,K_I,N,P<K_T,N,P,K_UP,N,P,K_dem\n");
    //fprintf(fp,"%4.2f,%4.2f,%4.2d,%4.2d,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f",
        Latitude[Lat],
        Longitude[Lon],
        MeteoYear[Day],
        MeteoDay[Day],
        Crop->st.Development,
        Crop->st.leaves,
        Crop->st.stems,
        Crop->st.storage,
        Crop->st.roots,
        Crop->st.LAI,
        WatBal->WaterStress,
        WatBal->st.Moisture,
        Rain[Lon][Lat][Day],
        WatBal->rt.Infiltration,
        WatBal->rt.Runoff,
        WatBal->rt.Loss,
        Crop->N_st.Indx,
        Crop->P_st.Indx,
        Crop->K_st.Indx,
        Site->st_N_tot,
        Site->st_P_tot,
        Site->st_K_tot,
        Crop->N_rt.Uptake,
        Crop->P_rt.Uptake,
        Crop->K_rt.Uptake,
        Crop->N_rt.Demand_lv + Crop->N_rt.Demand_st + Crop->N_rt.Demand_ro,
        Crop->P_rt.Demand_lv + Crop->P_rt.Demand_st + Crop->P_rt.Demand_ro,
        Crop->K_rt.Demand_lv + Crop->K_rt.Demand_st + Crop->K_rt.Demand_ro);
}
*/

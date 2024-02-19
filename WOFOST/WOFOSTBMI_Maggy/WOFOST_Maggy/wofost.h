#ifndef WOFOST_H
#define WOFOST_H

#include <time.h>
#include <stdio.h>

#define NR_VARIABLES_CRP    66
#define NR_TABLES_CRP       22
#define NR_VARIABLES_SITE       12
#define NR_TABLES_SITE          1
#define NR_VARIABLES_SOIL       12
#define NR_VARIABLES_SOIL_USED  6
#define NR_TABLES_SOIL          2
#define NR_VARIABLES_MANAGEMENT 9
#define NR_TABLES_MANAGEMENT    4
#define NUMBER_OF_TABLES        31
#define MAX_STRING             2048
#define METEO_LENGTH           36600  
#define DOMAIN_LENGTH          1000   

extern struct tm current_date;

typedef struct TBL {
    float x;
    float y;
    struct TBL *next;
    } TABLE;
        
typedef struct TBLD {
    int month;
    int day;
        float amount;
    struct TBLD *next;
    } TABLE_D;

typedef struct MANAGEMENT {
        /** Tables for fertilizer application and recovery fraction **/
        TABLE_D *N_Fert_table;
        TABLE_D *P_Fert_table;
        TABLE_D *K_Fert_table;
        TABLE_D *Irrigation;
        
        float N_Mins;
        float NRecoveryFrac;
        float P_Mins;
        float PRecoveryFrac;
        float K_Mins; 
        float KRecoveryFrac;
        float N_Uptake_frac;
        float P_Uptake_frac;
        float K_Uptake_frac;
        } Management;
extern Management *Mng;

typedef struct CONSTANTS {
        float MaxEvapWater;
        float MoistureFC;
        float MoistureWP;
        float MoistureSAT;
        float CriticalSoilAirC;
        float MaxPercolRTZ;
        float MaxPercolSubS;
        float MaxSurfaceStorge;
        float K0;
        } Constants;

typedef struct PARAMETERS {
          /** Tables for the Crop simulations **/
        TABLE *Roots;
        TABLE *Stems;
        TABLE *Leaves;
        TABLE *Storage;

        TABLE *VernalizationRate;
        TABLE *DeltaTempSum;
        TABLE *SpecificLeaveArea;
        TABLE *SpecificStemArea;
        TABLE *KDiffuseTb;
        TABLE *EFFTb;
        TABLE *MaxAssimRate; 
        TABLE *FactorAssimRateTemp;
        TABLE *FactorGrossAssimTemp;
        TABLE *FactorSenescence;
        TABLE *DeathRateStems;
        TABLE *DeathRateRoots; 
        
        /** Tables to account for the atmospheric CO2 concentration **/
        TABLE *CO2AMAXTB;
        TABLE *CO2EFFTB;
        TABLE *CO2TRATB;

        /** Tables for the maximum nutrient content in leaves as a function of DVS **/
        TABLE *N_MaxLeaves;
        TABLE *P_MaxLeaves;
        TABLE *K_MaxLeaves;

        /** Static Variables  **/
        /**  Emergence  **/
        float TempBaseEmergence;
        float TempEffMax;
        float TSumEmergence;                             

        /**  Phenology  **/
        int   IdentifyAnthesis; 
        float OptimumDaylength;             
        float CriticalDaylength;
        float SatVernRequirement;
        float BaseVernRequirement;
        float TempSum1;       
        float TempSum2; 
        float InitialDVS;
        float DevelopStageHarvest;

        /** Initial Values  **/
        float InitialDryWeight;
        float RelIncreaseLAI;

        /**  Green Area  **/
        float SpecificPodArea;
        float LifeSpan;
        float TempBaseLeaves;

        /** Conversion assimilates into biomass **/
        float ConversionLeaves;
        float ConversionStorage;
        float ConversionRoots;
        float ConversionStems;

        /** Maintenance Respiration **/
        float Q10;
        float RelRespiLeaves;
        float RelRespiStorage;
        float RelRespiRoots;
        float RelRespiStems;

        /** Death Rates  **/
        float MaxRelDeathRate;

        /** Water Use  **/
        float CorrectionTransp;
        float CropGroupNumber;
        float Airducts;

        /** Rooting **/
        float InitRootingDepth;
        float MaxIncreaseRoot;
        float MaxRootingDepth;

        /** Nutrients **/
        float DyingLeaves_NPK_Stress; 
        float DevelopmentStageNLimit; 
        float DevelopmentStageNT;
        float FracTranslocRoots;  
        float Opt_N_Frac;   
        float Opt_P_Frac;   
        float Opt_K_Frac;   
        float N_MaxRoots;   
        float N_MaxStems;   
        float P_MaxRoots;   
        float P_MaxStems;   
        float K_MaxRoots;   
        float K_MaxStems;   
        float NitrogenStressLAI;   
        float NLUE;   
        float Max_N_storage; 
        float Max_P_storage; 
        float Max_K_storage; 
        float N_lv_partitioning;  
        float NutrientStessSLA;   
        float N_ResidualFrac_lv;  
        float N_ResidualFrac_st;  
        float N_ResidualFrac_ro;  
        float P_ResidualFrac_lv;  
        float P_ResidualFrac_st;  
        float P_ResidualFrac_ro;  
        float K_ResidualFrac_lv;  
        float K_ResidualFrac_st;   
        float K_ResidualFrac_ro;   
        float TCNT;   
        float TCPT;   
        float TCKT;   
        float N_fixation; 
        } Parameters;


typedef struct STATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float Rain;
        float RootZoneMoisture;
        float Runoff;
        float SurfaceStorage;
        float Transpiration;
        float WaterRootExt;
        } States;
        

typedef struct RATES {
        double EvapWater;
        double EvapSoil;
        double Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float RootZoneMoisture;
        float Runoff;
        double Transpiration;
        float WaterRootExt;
        } Rates;
        
 
typedef struct NUTRIENT_RATES {
        float roots;
        float stems;
        float leaves;
        float storage;
        float Demand_lv;
        float Demand_st;
        float Demand_ro;
        float Demand_so;
        float Supply;
        float Transloc;
        float Transloc_lv;
        float Transloc_st;
        float Transloc_ro;
        float Uptake;
        float Uptake_lv;
        float Uptake_st;
        float Uptake_ro;
        float death_lv;
        float death_st;
        float death_ro;
        } nutrient_rates;
        

typedef struct NUTRIENT_STATES {
        float roots;
        float stems;
        float leaves;
        float storage;
        float Max_lv;
        float Max_st;
        float Max_ro;
        float Max_so;
        float Optimum_lv;
        float Optimum_st;
        float Indx;
        float Uptake;
        float Uptake_lv;
        float Uptake_st;
        float Uptake_ro;
        float death_lv;
        float death_st;
        float death_ro;
        float Avail;
        float Avail_lv;
        float Avail_st;
        float Avail_ro;
        
        } nutrient_states;
        

typedef struct GROWTH_RATES {
        float roots;
        float stems;
        float leaves;
        float LAIExp;
        float storage;
        float Development;
        float RootDepth;
        float vernalization;
} growth_rates;

typedef struct GROWTH_STATES {
        float roots;
        float stems;
        float leaves;
        float LAI;
        float LAIExp;
        float storage;
        float Development;
        float RootDepth;
        float RootDepth_prev;
        float vernalization;
        } growth_states;

typedef struct DYING_STATES {
        float roots;
        float stems;
        float leaves;
        } dying_states; 
        
typedef struct DYING_RATES {
        float roots;
        float stems;
        float leaves;
        } dying_rates; 


typedef struct GREEN {
    float weight;
    float age;
    float area;
    struct GREEN *next;
    } Green;
        

typedef struct PLANT {
        int Emergence;
        int Sowing;
        int GrowthDay;       
        float NPK_Indx;
        float NutrientStress;
        float DaysOxygenStress;
        float TSumEmergence;
        float fac_ro;
        float fac_lv;
        float fac_st;
        float fac_so;
        
        double ESO; 
        double OWE; 
        double POT;
        double ET;
        double AESO;
        double ATRA;
        
        Parameters prm;
        
        growth_rates  rt;
        growth_states st;
        dying_rates   drt;
        dying_states  dst;
        
        nutrient_states N_st;
        nutrient_states P_st;
        nutrient_states K_st;
        
        nutrient_rates N_rt;
        nutrient_rates P_rt;
        nutrient_rates K_rt;            
        
        Green *LeaveProperties;
    } Plant;    
extern Plant *Crop; /* Place holder for the current crop simulations */


typedef struct SOIL {
        float DaysSinceLastRain;
        float SoilMaxRootingDepth;
        float WaterStress;
        float InfPreviousDay;
        
        /* Tables for Soil */
        TABLE *VolumetricSoilMoisture;
        TABLE *HydraulicConductivity; /* currently not used */
        
        Constants ct;
        States st;
        Rates rt;
        } Soil;
extern Soil *WatBal; /* Place holder for the current water balance simulations */


typedef struct FIELD {
        /* Water related parameters */
        float FlagGroundWater;
        float InfRainDependent;
        float FlagDrains;
        float MaxSurfaceStorage;     
        float InitSoilMoisture;
        float GroundwaterDepth;
        float DD;
        float SoilLimRootDepth;
        float NotInfiltrating;
        float SurfaceStorage;
        float MaxInitSoilM;
        
        /* Mineral states and rates */
        float st_N_tot;
        float st_P_tot;
        float st_K_tot;

        float st_N_mins;
        float st_P_mins;
        float st_K_mins;

        float rt_N_tot;
        float rt_P_tot;
        float rt_K_tot;

        float rt_N_mins;
        float rt_P_mins;
        float rt_K_mins;
        
        /** Table for the fraction of precipitation that does not infiltrate **/
        TABLE *NotInfTB;
        } Field;
extern Field *Site; /* Place holder for the current site simulations */


/* date structure */
typedef struct {
    unsigned short int day;     
    unsigned short int day_in_year;
    unsigned short int month;
    unsigned int dayseconds;
    int year;
        
} dmy_struct;



/* Place holder for a simulation unit */
typedef struct SIMUNIT {
        Plant *crp;
        Field *ste;
        Management *mng;
        Soil  *soil;
        int emergence;
        int file;
        char start[MAX_STRING];
        char output[MAX_STRING];
        float se_hat;
        float theta_sat;
        float theta_res;
        
        /* Statistics */
        float twso[40];
        struct SIMUNIT *next;
    } SimUnit; 
extern SimUnit *Grid;



enum{
    WEATHER_TMIN,
    WEATHER_TMAX,
    WEATHER_RADIATION,
    WEATHER_RAIN,
    WEATHER_WINDSPEED,
    WEATHER_VAPOUR,
    WEATHER_NTYPES
};

typedef struct INPUT {
    char cropfile[MAX_STRING];
    char sitefile[MAX_STRING];
    char management[MAX_STRING];
    char soilfile[MAX_STRING];
    char start[MAX_STRING];
    char output[MAX_STRING];
    int emergence;
    int file;
    struct INPUT *next;
    } Input;
extern Input *data;

typedef struct WEATHER {
        char mask[MAX_STRING];
        char planting_date[MAX_STRING];
        char file[WEATHER_NTYPES][MAX_STRING];
        char type[WEATHER_NTYPES][MAX_STRING];
        char var[WEATHER_NTYPES][MAX_STRING];
        int StartYear;
        int EndYear;
        size_t nlat;
        size_t nlon;
        size_t ntime;
        struct WEATHER *next;
        } Weather;
extern Weather *Meteo; /* Place holder for the meteo filenames and lat/lon */

/** Meteorological Variables  **/
extern int Station, Year;
extern int MeteoYear[METEO_LENGTH];
extern int MeteoDay[METEO_LENGTH];
extern float CO2;
extern double Longitude[DOMAIN_LENGTH], Latitude[DOMAIN_LENGTH];
extern int **Mask;
extern int **Planting_date;
extern float **Altitude;
extern float **AngstA;
extern float **AngstB;
extern float ***Tmin;
extern float ***Tlow;
extern float ***Tmax;
extern float ***Radiation;
extern float ***Rain;
extern float ***Windspeed;
extern float ***Vapour;
extern SimUnit ***Grids;
extern FILE **output;


extern int dag;
/* Time step */
extern int Step;



             
#endif  


#include <stdio.h>










#define roundz(x,d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))

//float Step;
int MeteoYear[METEO_LENGTH], MeteoDay[METEO_LENGTH];
float **Altitude, **AngstA, **AngstB;
float ***Tmin, ***Tmax, ***Radiation, ***Rain, ***Windspeed, ***Vapour,***Tlow;
SimUnit ***Grids;
int Station, Year, **Mask, **Planting_date;
double Longitude[DOMAIN_LENGTH], Latitude[DOMAIN_LENGTH];



int GetMeteoData(Weather* meteo)
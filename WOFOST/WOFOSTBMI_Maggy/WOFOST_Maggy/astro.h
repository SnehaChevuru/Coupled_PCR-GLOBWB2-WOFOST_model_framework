#ifndef ASTRO_H
#define ASTRO_H

extern float AtmosphTransm;
extern float AngotRadiation;
extern float Daylength;
extern float PARDaylength;
extern float SinLD;
extern float CosLD; 
extern float DiffRadPP;
extern float DSinBE;

extern float min(float a, float b);
extern float max(float a, float b);
extern float limit(float a, float b, float c);
extern float Afgen();

#endif	// ASTRO_H


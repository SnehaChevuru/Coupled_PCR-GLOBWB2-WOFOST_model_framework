
#ifndef WOFOST_BMI_H_INCLUDED
#define WOFOST_BMI_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "bmi.h"
#include "wofost.h"    

Bmi* register_bmi_wofost(Bmi *model);
SimUnit* new_bmi_wofost(void);    
void wofost_initialize_config(const char *filename);
int wofost_update();
int wofost_finalize();

#ifdef __cplusplus
}
#endif

#endif /* WOFOST_BMI_H */


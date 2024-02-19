# Coupled_PCR-GLOBWB2-WOFOST_model_framework
A new fully coupled PCR-GLOBWB2 - WOFOST model framework is developed to include the feedbacks between crop growth and hydrology. 

Install the BMI WOFOST model
1. First build and install the WOFOST BMI model
2. Wrap the model with babelizer (Babelizer helps to run the BMI wofost in python session)

To build and install the WOFOST BMI model:
- CMakeLists.txt (change the folder and sub directory name accordingly). This helps to install the BMI in wofost model
- bmiwofostc.pc.cmake 
- wofostc.pc.cmake
make install

Wrap the model with babelizer
- babel_wofostc.toml
use: babelize init babel_wofostc.toml to activate the wrapper
Install the dependencies into conda environment
make install

Download the PCRGLOBWB2 BMI model

To activate the BMI PCRGLOBWB2-WOFOST:

from model import bmiPcrglobwb

config_pcr = "/setup_conus_5arcmin.ini"

pcr = bmiPcrglobwb.BmiPCRGlobWB()

pcr.initialize(config_pcr)

from pymt_wofost import WOFOST

config_wofost = "/list.txt"

wofost = WOFOST()

wofost.initialize(config_wofost)

pcr.update   (run for 1 time step)  #### refer to ini file ####

wofost.update

pcr.finalize()

wofost.finalize()


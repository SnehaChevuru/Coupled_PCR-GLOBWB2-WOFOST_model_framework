import numpy as np

# Activating the hydrology and crop models 
from model import bmiPcrglobwb
config_pcr = "/quanta1/home/chevu001/PCR-WOFOST/TW/Pcrglobwb/setup_conus_5arcmin.ini"
pcr = bmiPcrglobwb.BmiPCRGlobWB()
pcr.initialize(config_pcr)


from pymt_Maggy import WOFOST_MTW
config_Maize = "/quanta1/home/chevu001/PCR-WOFOST/TW/Maggy/WOFOSTBMI_Maggy/WOFOST_Maggy/list.txt"
Maize_wofost = WOFOST_MTW()
Maize_wofost.initialize(config_Maize)

from pymt_Saggy import WOFOST_STW
config_Soybean = "/quanta1/home/chevu001/PCR-WOFOST/TW/Saggy/WOFOSTBMI_Saggy/WOFOST_Saggy/list_sb.txt"
Soybean_wofost = WOFOST_STW()
Soybean_wofost.initialize(config_Soybean)

from pymt_Weggy import WOFOST_WTW
config_Wheat = "/quanta1/home/chevu001/PCR-WOFOST/TW/Weggy/WOFOSTBMI_Weggy/WOFOST_Weggy/list_ww.txt"
Wheat_wofost = WOFOST_WTW()
Wheat_wofost.initialize(config_Wheat)

# end_time is used to retrive the end of time step
end_time = Maize_wofost.get_end_time()
end_time = int(end_time)

size = Maize_wofost.get_grid_size(0)
print(size)


crop_types = ["irrMaize","irrSoybean","irrWheat"]
wofost_models = dict(zip(crop_types,[Maize_wofost,Soybean_wofost,Wheat_wofost]))

## modules ##

# PCR variables
# get soil Parameters before the update 
#depths 
dep1 = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"thickUpp").astype("float32")) for crop_type in crop_types)
dep2 = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"thickLow").astype("float32")) for crop_type in crop_types)


#volumteric moisture content
sat_upp = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"satVolMoistContUpp").astype("float32")) for crop_type in crop_types)
sat_low = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"satVolMoistContLow").astype("float32")) for crop_type in crop_types)
res_upp = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"resVolMoistContUpp").astype("float32")) for crop_type in crop_types)
res_low = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"resVolMoistContLow").astype("float32")) for crop_type in crop_types)

#soil variables
fc_upp = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"effSatAtFieldCapUpp").astype("float32")) for crop_type in crop_types)
fc_low = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"effSatAtFieldCapLow").astype("float32")) for crop_type in crop_types)
wp_upp = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"effSatAtWiltPointUpp").astype("float32")) for crop_type in crop_types)
wp_low = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type].parameters,"effSatAtWiltPointLow").astype("float32")) for crop_type in crop_types)

sat = {}
res = {}
fc = {}
wp = {}
theta_fc = {}
theta_wp = {}
RD = {}
for crop_type in crop_types:
    
    ## the variables goes to WOFOST !! initialise the soil parameters !! ##   
    sat[crop_type] = (((sat_upp[crop_type]*dep1[crop_type])+(sat_low[crop_type]*dep2[crop_type]))/(dep1[crop_type]+dep2[crop_type])).reshape(size)
    res[crop_type] = (((res_upp[crop_type]*dep1[crop_type])+(res_low[crop_type]*dep2[crop_type]))/(dep1[crop_type]+dep2[crop_type])).reshape(size)
    fc[crop_type] = (((fc_upp[crop_type]*dep1[crop_type])+(fc_low[crop_type]*dep2[crop_type]))/(dep1[crop_type]+dep2[crop_type])).reshape(size)
    wp[crop_type] = (((wp_upp[crop_type]*dep1[crop_type])+(wp_low[crop_type]*dep2[crop_type]))/(dep1[crop_type]+dep2[crop_type])).reshape(size)
    theta_fc[crop_type] = sat[crop_type] *fc[crop_type]
    theta_wp[crop_type] = sat[crop_type] *wp[crop_type]

    RD[crop_type] = np.full((300,840),0).astype("float32")
    
    
# loop begins
for i in range(end_time):    
    ## getting the values ##
    
    pcr.pre_update()
    
    ### Meteo_variables get from PCR ###
    P = pcr.get_value(pcr.model.meteo,"precipitation").astype("float32")
    TMax = pcr.get_value(pcr.model.meteo,"air_temperature_max").astype("float32")
    TMin = pcr.get_value(pcr.model.meteo,"air_temperature_min").astype("float32")
    WS = pcr.get_value(pcr.model.meteo,"wind_speed_10m").astype("float32")
    SR = pcr.get_value(pcr.model.meteo,"shortwave_radiation").astype("float32")
    VP = pcr.get_value(pcr.model.meteo,"vapour_pressure").astype("float32")
    
    #### Calculating degree of stauration for Maize crop #####
    stor_upp = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type],"storUpp").astype("float32")) for crop_type in crop_types)
    stor_low = dict((crop_type, pcr.get_value(pcr.model.landSurface.landCoverObj[crop_type],"storLow").astype("float32")) for crop_type in crop_types)

    f1 = {}
    f2 = {}
    theta_hat = {}
    theta_sat = {}
    theta_res = {}
    se_hat = {}
    RD1 = {}
    AE = {}
    PE = {}
    AT = {}
    WE = {}
    ET = {}
    
    for crop_type, wofost_model in wofost_models.items():
        
        f1[crop_type] = np.divide(np.minimum(RD[crop_type],dep1[crop_type]*100),RD[crop_type], out = np.zeros_like(dep1[crop_type]), where=RD[crop_type] !=0)
        f1[crop_type] = np.where(RD[crop_type] ==0, 1, f1[crop_type])
        f2[crop_type] = 1-f1[crop_type]
        
        ## Volumetric MC variables (degree of saturation) ##
        theta_hat[crop_type] = f1[crop_type]*(res_upp[crop_type] + (stor_upp[crop_type]/dep1[crop_type])) + f2[crop_type] *(res_low[crop_type] + (stor_low[crop_type]/dep2[crop_type]))
        theta_sat[crop_type] = (f1[crop_type]*sat_upp[crop_type]) + (f2[crop_type]*sat_low[crop_type])
        theta_res[crop_type] = (f1[crop_type]*res_upp[crop_type]) + (f2[crop_type]*res_low[crop_type])

        # the variable se_hat (degree of saturation) goes to WOFOST
        se_hat[crop_type] = ((theta_hat[crop_type]-theta_res[crop_type])/(theta_sat[crop_type]-theta_res[crop_type])).reshape(size)   # getting the values
        
        ### set the soil parameters ###
        wofost_model.set_value("soil_moisture",se_hat[crop_type]) 
        wofost_model.set_value("saturation",sat[crop_type])
        wofost_model.set_value("residual",res[crop_type]) 
        wofost_model.set_value("field_capacity",theta_fc[crop_type])
        wofost_model.set_value("wilting_point",theta_wp[crop_type])
    
        ### set the meteo variables ###
        wofost_model.set_value("precipitation",P) 
        wofost_model.set_value("temperature_max",TMax)
        wofost_model.set_value("temperature_min",TMin) 
        wofost_model.set_value("wind_speed",WS)
        wofost_model.set_value("shortwave_radiation",SR)
        wofost_model.set_value("vapour_pressure",VP)
        
        wofost_model.update()
    
        ### WOFOST get variables ###
        buffer = np.zeros(size)
        RD[crop_type] = np.reshape(wofost_model.get_value("root_depth",buffer),(300,840)).astype("float32") # lat and lon
        
    
        default_value = 1e20
    
        RD1[crop_type] = RD[crop_type] * 0.01
        RD1[crop_type][RD1[crop_type] == 0] = default_value
        
        ## Vegetative fluxes ##
    
        AE[crop_type] = np.reshape(wofost_model.get_value("ActBSevaporation",buffer),(300,840)).astype("float32")
        AE[crop_type][AE[crop_type] ==1e18] = default_value
        
        
        PE[crop_type] = np.reshape(wofost_model.get_value("PotBSevaporation",buffer),(300,840)).astype("float32")
        PE[crop_type][PE[crop_type] ==1e18] = default_value
        
        
        AT[crop_type] = np.reshape(wofost_model.get_value("Acttranspiration",buffer),(300,840)).astype("float32")
        AT[crop_type][AT[crop_type] ==1e18] = default_value
        
        
        WE[crop_type] = np.reshape(wofost_model.get_value("OpenWevaporation",buffer),(300,840)).astype("float32")
        WE[crop_type][WE[crop_type] ==1e18] = default_value
        
        
        ET[crop_type] = np.reshape(wofost_model.get_value("PotET",buffer),(300,840)).astype("float32")
        ET[crop_type][ET[crop_type] ==1e18] = default_value
        
        
        ### PCR set vegetative fluxes ###
        
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"actBareSoilEvap",AE[crop_type])
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"potBareSoilEvap",PE[crop_type])
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"actTranspiration",AT[crop_type])
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"openWaterEvap",WE[crop_type])
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"totalPotET",ET[crop_type])
        pcr.set_value(pcr.model.landSurface.landCoverObj[crop_type],"root_depth", RD1[crop_type] )
    
       
    pcr.update()
       
    
# finalizing (closing) the models   

Maize_wofost.finalize()
Soybean_wofost.finalize()
Wheat_wofost.finalize()
pcr.finalize()










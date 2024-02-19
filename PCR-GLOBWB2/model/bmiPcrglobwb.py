#! /usr/bin/env python
from model.configuration import Configuration
from model.pcrglobwb import PCRGlobWB
import pcraster as pcr
import numpy as np
from model.currTimeStep import ModelTime
import sys
import logging
from model.reporting import Reporting

#from model.imagemean import downsample
from model.bmi import Bmi
from model.bmi import EBmi
from model.bmi import BmiGridType
import datetime
import model.variable_list as variable_list
import os
try:
  from model.imagemean import downsample
  HAVE_DOWNSAMPLE=True
except:
  HAVE_DOWNSAMPLE=False

logger = logging.getLogger(__name__)


#import model.ETPFunctions as refPotET

class BmiPCRGlobWB(EBmi):

    epoch_start_date = datetime.date(1901, 1, 1)

    scalar_variables = ["flood_innundation_depth"]

    # we use the same epoch as pcrglobwb netcdf reporting
    @staticmethod
    def days_since_industry_epoch(model_time):
        return (model_time - BmiPCRGlobWB.epoch_start_date).days

    @staticmethod
    def in_modeltime(days_since_industry_epoch):
        return BmiPCRGlobWB.epoch_start_date + datetime.timedelta(days=days_since_industry_epoch)

    def __init__(self):
        self.configuration = None
        self.model = None
        self.model_time = None
        self.shape = None
        self.reporting = None
        self._use_interface_forcings=False

    def update_output(self):
        self.output = {}
        if self.model is None:
            return
        submodels = [self.model,
                     self.model.routing,
                     self.model.routing.WaterBodies,
                     self.model.landSurface,
                     self.model.meteo,
                     self.model.groundwater]

        for key in variable_list.netcdf_short_name:
            for submodel in submodels:
                if hasattr(submodel, key):
                    self.output[key] = submodel
                    break

    def calculate_shape(self):
        return pcr.clone().nrRows(), pcr.clone().nrCols()

    # BMI initialize (as a single step)
    def initialize(self, filename):
        self.initialize_config(filename)
        self.initialize_model()

    # EBMI initialize (first step of two)
    def initialize_config(self, filename):
        logger.info("PCRGlobWB: initialize_config")
        try:
            self.configuration = Configuration(filename, relative_ini_meteo_paths = True)
            pcr.setclone(self.configuration.cloneMap)

            # set start and end time based on configuration
            self.model_time = ModelTime()
            self.model_time.getStartEndTimeSteps(self.configuration.globalOptions['startTime'],
                                                 self.configuration.globalOptions['endTime'])
            self.model_time.update(0)
            self.shape = self.calculate_shape()
            logger.info("Shape of maps is %s", str(self.shape))
            
            self.model = None
        except:
            import traceback
            traceback.print_exc()
            raise

    # EBMI initialize (second step of two)
    def initialize_model(self):
        if self.model is not None:
            # already initialized
            return
        try:
            logger.info("PCRGlobWB: initialize_model")#, source dir %s is not used" % source_directory)
            initial_state = None

            self.model = PCRGlobWB(self.configuration, self.model_time, initial_state)
            self.model.meteo.temperature = pcr.scalar(0.0)
            self.model.meteo.precipitation = pcr.scalar(0.0)
            self.model.meteo.referencePotET = pcr.scalar(0.0)

            self.model.landSurface.totalPotET = pcr.scalar(0.0)
            self.model.landSurface.actualET = pcr.scalar(0.0)
            self.model.landSurface.storUppTotal = pcr.scalar(0.0)
            self.model.landSurface.storLowTotal = pcr.scalar(0.0)
            self.model.landSurface.interceptEvap = pcr.scalar(0.0)
            self.model.landSurface.actSnowFreeWaterEvap = pcr.scalar(0.0)
            self.model.landSurface.openWaterEvap = pcr.scalar(0.0)
            self.model.landSurface.actBareSoilEvap = pcr.scalar(0.0)
            self.model.landSurface.actTranspiTotal = pcr.scalar(0.0)
            self.model.landSurface.actTranspiUppTotal = pcr.scalar(0.0)
            self.model.landSurface.actTranspiLowTotal = pcr.scalar(0.0)
            self.model.landSurface.directRunoff = pcr.scalar(0.0)
            self.model.landSurface.interflowTotal = pcr.scalar(0.0)
            self.model.landSurface.infiltration = pcr.scalar(0.0)
            self.model.landSurface.gwRecharge = pcr.scalar(0.0)
            self.model.landSurface.satDegUppTotal = pcr.scalar(0.0)
            self.model.landSurface.satDegLowTotal = pcr.scalar(0.0)
            self.model.landSurface.desalinationAbstraction = pcr.scalar(0.0)
            self.model.landSurface.actSurfaceWaterAbstract = pcr.scalar(0.0)
            self.model.landSurface.irrGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.nonIrrGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.totalPotentialGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.domesticGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.domesticNettoDemand = pcr.scalar(0.0)
            self.model.landSurface.industryGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.industryNettoDemand = pcr.scalar(0.0)
            self.model.landSurface.livestockGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.livestockNettoDemand = pcr.scalar(0.0)
            self.model.landSurface.totalSto = pcr.scalar(0.0)
            self.model.landSurface.allocSurfaceWaterAbstract = pcr.scalar(0.0)
            self.model.landSurface.desalinationAllocation = pcr.scalar(0.0)
            self.model.landSurface.snowMelt = pcr.scalar(0.0)
            self.model.landSurface.netLqWaterToSoil = pcr.scalar(0.0)
            self.model.landSurface.nonIrrReturnFlow = pcr.scalar(0.0)
            self.model.landSurface.totalPotentialMaximumGrossDemand = pcr.scalar(0.0)
            self.model.landSurface.irrGrossDemandPaddy = pcr.scalar(0.0)
            self.model.landSurface.irrGrossDemandNonPaddy = pcr.scalar(0.0)
            self.model.landSurface.irrGrossDemandMaize = pcr.scalar(0.0)
            self.model.landSurface.domesticWaterWithdrawal = pcr.scalar(0.0)
            self.model.landSurface.industryWaterWithdrawal = pcr.scalar(0.0)
            self.model.landSurface.livestockWaterWithdrawal = pcr.scalar(0.0)
            self.model.landSurface.domesticReturnFlowFraction = pcr.scalar(0.0)
            self.model.landSurface.industryReturnFlowFraction = pcr.scalar(0.0)
            self.model.landSurface.livestockReturnFlowFraction = pcr.scalar(0.0)
            if self.model.landSurface.numberOfSoilLayers == 3:
                self.model.landSurface.storUpp000005 = pcr.scalar(0.0)
                self.model.landSurface.storUpp005030 = pcr.scalar(0.0)
                self.model.landSurface.storLow030150 = pcr.scalar(0.0)
                self.model.landSurface.satDegUpp000005 = pcr.scalar(0.0)

            if self.model.landSurface.includeIrrigation:
                self.model.landSurface.landCoverObj['irrPaddy'].netLqWaterToSoil = pcr.scalar(0.0)
                self.model.landSurface.landCoverObj['irrNonPaddy'].netLqWaterToSoil = pcr.scalar(0.0)
                
                self.model.landSurface.landCoverObj['irrPaddy'].actualET = pcr.scalar(0.0)
                self.model.landSurface.landCoverObj['irrNonPaddy'].actualET = pcr.scalar(0.0)
                
                self.model.landSurface.landCoverObj['irrPaddy'].actTranspiTotal = pcr.scalar(0.0)
                self.model.landSurface.landCoverObj['irrNonPaddy'].actTranspiTotal = pcr.scalar(0.0)
                

            self.model.groundwater.nonFossilGroundwaterAbs = pcr.scalar(0.0)
            self.model.groundwater.fossilGroundwaterAbstr = pcr.scalar(0.0)
            self.model.groundwater.allocNonFossilGroundwater = pcr.scalar(0.0)
            self.model.groundwater.unmetDemand = pcr.scalar(0.0)

            self.model.routing.waterBodyEvaporation = pcr.scalar(0.0)
            self.model.routing.waterBodyPotEvap = pcr.scalar(0.0)
            self.model.routing.runoff = pcr.scalar(0.0)
            self.model.routing.disChanWaterBody = pcr.scalar(0.0)
            self.model.routing.dynamicFracWat = pcr.scalar(0.0)
            self.model.routing.local_input_to_surface_water = pcr.scalar(0.0)
            self.model.routing.nonIrrWaterConsumption = pcr.scalar(0.0)
            self.model.routing.WaterBodies.waterBodyIds = pcr.scalar(0.0)
            self.model.routing.WaterBodies.waterBodyStorage = pcr.scalar(0.0)
            if self.model.routing.floodPlain:
                self.model.routing.floodDepth = pcr.scalar(0.0)
                self.model.routing.floodVolume = pcr.scalar(0.0)
                self.model.routing.channelStorageCapacity = pcr.scalar(0.0)
                self.model.routing.channelStorage = pcr.scalar(0.0)
            self.reporting = Reporting(self.configuration, self.model, self.model_time)
            self.reporting.post_processing()

            logger.info("Shape of maps is %s", str(self.shape))
            logger.info("PCRGlobWB Initialized")
        except:
            import traceback
            traceback.print_exc()
            raise

    def _calc_refPotET(self):
        # calculate referencePotET
        # see the comment in update of meteo - this should move there
        if self.model.meteo.refETPotMethod == 'Hamon': 
            self.model.meteo.referencePotET = refPotET.HamonPotET(self.model.meteo.temperature,
                                                      self.model_time.doy,
                                                      self.model.meteo.latitudes)


    def pre_update(self):

        '''

pre_update: function of the BmiPCRGlobWB class that updates the time step and \
the date of the modelTime instance for the actual update, reads in the forcing \
for the meteo module, so that it can be coupled to other models by BMI and \
initializes any fluxes per land cover type that can be imposed to PCR-GLOBWB \
from external models such as WOFOST.
The function does not require any input and returns None.

'''
        # update the timestep and the correspnoding date information
        timestep = self.model_time.timeStepPCR
        self.model_time.update(timestep + 1)

        # read in the forcings for the meteo module
        self.model.read_forcings()        

        # initialize any imposed fluxes per land cover type
        self.model.landSurface.initialize_imposed_landcover_variables()
            
        # returns None
        return None

       
    def update(self):
        timestep = self.model_time.timeStepPCR
        if self.model is None:
            raise ValueError("Model has not been initialized, unable to start time stepping")
        self.model_time.update(timestep )
               
        self.model.update(report_water_balance=True)
        if self.reporting is not None:
            
            self.reporting.report()

    def update_until(self, time):
        while self.get_current_time() + 0.001 < time:
            self.update()

    def update_frac(self, time_frac):
        raise NotImplementedError("pcrglobwb does not support fractional time steps")

    def finalize(self):
        self.configuration = None
        self.model = None
        self.model_time = None
        self.shape = None
        self.reporting = None

    def get_component_name(self):
        return "pcrglobwb"

    def get_input_var_names(self):
        input_var_names=['channel_storage']
        
        if self.model.landSurface.numberOfSoilLayers == 3:
            input_var_names.extend(["near_surface_soil_saturation_degree"])
        if self.model.landSurface.numberOfSoilLayers == 2:
            input_var_names.extend(["upper_soil_saturation_degree"])
        if self._use_interface_forcings:
            input_var_names.extend(["precipitation", "temperature"])
        return input_var_names

    def get_output_var_names(self):
        netcdf_short_name=variable_list.netcdf_short_name.copy()      
        if self.model.landSurface.numberOfSoilLayers != 3:
            netcdf_short_name.pop("satDegUppSurface")
            netcdf_short_name.pop("storUppSurface")
            netcdf_short_name.pop("storUpp000005")
            netcdf_short_name.pop("storUpp005030")
            netcdf_short_name.pop("storLow030150")
        if not self.model.routing.floodPlain:
            netcdf_short_name.pop("floodDepth")
            netcdf_short_name.pop("floodVolume")
        if not "land_surface_water_balance" in self.reporting.variables_for_report:
            netcdf_short_name.pop("land_surface_water_balance")
# modflow variables
        netcdf_short_name.pop("groundwaterHeadLayer1")
        netcdf_short_name.pop("groundwaterHeadLayer2")
        netcdf_short_name.pop("groundwaterDepthLayer1")
        netcdf_short_name.pop("groundwaterDepthLayer2")
        netcdf_short_name.pop("groundwaterHead")
        netcdf_short_name.pop("groundwaterDepth")
        netcdf_short_name.pop("relativeGroundwaterHead")
        netcdf_short_name.pop("groundwaterVolumeEstimate")
        netcdf_short_name.pop("groundwaterThicknessEstimate")
        netcdf_short_name.pop("top_uppermost_layer")
        netcdf_short_name.pop("bottom_uppermost_layer")
        netcdf_short_name.pop("bottom_lowermost_layer")
# random stuff not available/implemented (yet?)
        netcdf_short_name.pop("test")
        netcdf_short_name.pop("accuRunoff")
        netcdf_short_name.pop("accuTotalRunoff")
        netcdf_short_name.pop("irrWaterConsumption")
        netcdf_short_name.pop("irrReturnFlow")
        return netcdf_short_name.values()

    def get_var_type(self, var_name):
        if var_name == "soil_layer_count":
            return np.int32
        return np.float32

    def get_var_units(self, var_name):
        attribute = [n for n in variable_list.netcdf_short_name if variable_list.netcdf_short_name[n] == var_name][0]
        return variable_list.netcdf_unit[attribute]

    def get_var_rank(self, long_var_name):
        return 1

    def get_var_size(self, long_var_name):
        return self.get_var_itemsize(long_var_name)

    def get_var_itemsize(self, long_var_name):
        return self.get_var_type(long_var_name).itemsize

    def get_var_nbytes(self, long_var_name):
        return np.prod(self.get_grid_shape(long_var_name)) * self.get_var_itemsize(long_var_name)

    def get_start_time(self):
        return self.days_since_industry_epoch(self.model_time.startTime)

    def get_current_time(self):
        return self.days_since_industry_epoch(self.model_time.currTime)

    def get_end_time(self):
        return self.days_since_industry_epoch(self.model_time.endTime)

    def get_time_step(self):
        return 1

    # TODO: Add time zone info?
    def get_time_units(self):
        return "days since " + str(BmiPCRGlobWB.epoch_start_date)

    # TODO: Raises exception when attribute is missing, fix this
    def get_value_old(self, var_name):
        logger.info("getting value for var %s", var_name)
        attribute = [n for n in variable_list.netcdf_short_name if variable_list.netcdf_short_name[n] == var_name][0]
        #pcrdata = getattr(self.reporting, attribute)
              
        varlist = ["effSatAtFieldCapUpp","effSatAtFieldCapLow","effSatAtWiltPointUpp","effSatAtWiltPointLow","kSatUpp","kSatLow",\
                                "satVolMoistContUpp","satVolMoistContLow","resVolMoistContUpp","resVolMoistContLow","thickUpp","thickLow"]
                                
        storlist = ['storUpp', 'storLow']
        if (attribute in varlist):
        
            pcrdata = getattr(self.model.landSurface.landCoverObj['irrMaize'].parameters, attribute)
        
        else:
            
            if (attribute in storlist):
                
                pcrdata = getattr(self.model.landSurface.landCoverObj['irrMaize'], attribute)
            else:
                     
                pcrdata = getattr(self.model.meteo, attribute) 
        
        if var_name in BmiPCRGlobWB.scalar_variables:
            
            return var_name
        remasked = pcr.ifthen(self.model.landmask, pcr.cover(pcrdata, 0.0))
        pcr.report(pcrdata, "value.map")
        pcr.report(remasked, "remasked.map")
        return pcr.pcr2numpy(remasked, np.NaN)
        
    def get_value(self, module, var_name):
        
        logger.info("getting value for var %s", var_name)
        
        pcrdata = getattr(module, var_name)
    
        remasked = pcr.ifthen(self.model.landmask, pcr.cover(pcrdata, 0.0))
        pcr.report(pcrdata, "value.map")
        pcr.report(remasked, "remasked.map")
        return pcr.pcr2numpy(remasked, np.NaN)
    
    def get_value_from_meteo(self,module,var_name):
    
        logger.info("getting value for var %s", var_name)
        #attribute = [n for n in variable_list.netcdf_short_name if variable_list.netcdf_short_name[n] == var_name][0]
        pcrdata = getattr(module, var_name)
        
        remasked = pcr.ifthen(self.model.landmask, pcr.cover(pcrdata, 0.0))
        pcr.report(pcrdata, "value.map")
        pcr.report(remasked, "remasked.map")
        return pcr.pcr2numpy(remasked, np.NaN)
        
    def get_value_from_landcover(self,module,var_name):
    
        logger.info("getting value for var %s", var_name)
        #attribute = [n for n in variable_list.netcdf_short_name if variable_list.netcdf_short_name[n] == var_name][0]
        
        pcrdata = getattr(module, var_name)
        
        remasked = pcr.ifthen(self.model.landmask, pcr.cover(pcrdata, 0.0))
        pcr.report(pcrdata, "value.map")
        pcr.report(remasked, "remasked.map")
        return pcr.pcr2numpy(remasked, np.NaN)
    
    def get_value_at_indices(self, long_var_name, indices):
        return self.get_value(long_var_name).take(indices)

    def set_satDegUpp000005(self, src):
        mask = np.isnan(src)
        src[mask] = 1e20
        observed_satDegUpp000005 = pcr.numpy2pcr(pcr.Scalar, src, 1e20)

        pcr.report(observed_satDegUpp000005, "observed.map")

        constrained_satDegUpp000005 = pcr.min(1.0, pcr.max(0.0, observed_satDegUpp000005))

        pcr.report(constrained_satDegUpp000005, "constrained.map")

        pcr.report(self.model.landSurface.satDegUpp000005, "origmap.map")
        diffmap = constrained_satDegUpp000005 - self.model.landSurface.satDegUpp000005
        pcr.report(diffmap, "diffmap.map")

        # ratio between observation and model
        ratio_between_observation_and_model = pcr.ifthenelse(self.model.landSurface.satDegUpp000005 > 0.0,
                                                             constrained_satDegUpp000005 / \
                                                             self.model.landSurface.satDegUpp000005, 0.0)

        # updating upper soil states for all lad cover types
        for coverType in self.model.landSurface.coverTypes:
            # correcting upper soil state (storUpp000005)
            self.model.landSurface.landCoverObj[coverType].storUpp000005 *= ratio_between_observation_and_model

            # if model value = 0.0, storUpp000005 is calculated based on storage capacity (model parameter) and observed saturation degree   
            self.model.landSurface.landCoverObj[coverType].storUpp000005 = pcr.ifthenelse(
                self.model.landSurface.satDegUpp000005 > 0.0, \
                self.model.landSurface.landCoverObj[coverType].storUpp000005, \
                constrained_satDegUpp000005 * self.model.landSurface.landCoverObj[coverType].parameters.storCapUpp000005)
            # this was self.model.landSurface.parameters.storCapUpp000005 
            # correct for any scaling issues (value < 0 or > 1 do not make sense
            self.model.landSurface.landCoverObj[coverType].storUpp000005 = pcr.min(1.0, pcr.max(0.0,
                                                                                                self.model.landSurface.landCoverObj[
                                                                                                    coverType].storUpp000005))

    def set_satDegUpp(self, src):
        mask = np.isnan(src)
        src[mask] = 1e20
        observed_satDegUpp = pcr.numpy2pcr(pcr.Scalar, src, 1e20)

        pcr.report(observed_satDegUpp, "observed.map")

        constrained_satDegUpp = pcr.min(1.0, pcr.max(0.0, observed_satDegUpp))

        pcr.report(constrained_satDegUpp, "constrained.map")

        pcr.report(self.model.landSurface.satDegUpp, "origmap.map")
        diffmap = constrained_satDegUpp - self.model.landSurface.satDegUpp
        pcr.report(diffmap, "diffmap.map")

        # ratio between observation and model
        ratio_between_observation_and_model = pcr.ifthenelse(self.model.landSurface.satDegUpp > 0.0,
                                                             constrained_satDegUpp / \
                                                             self.model.landSurface.satDegUpp, 0.0)

        # updating upper soil states for all lad cover types
        ls=self.model.landSurface
        for coverType in ls.coverTypes:
            lco=ls.landCoverObj[coverType]
            # correcting upper soil state (storUpp)
            lco.storUpp *= ratio_between_observation_and_model

            # if model value = 0.0, storUpp000005 is calculated based on storage capacity (model parameter) and observed saturation degree   
            lco.storUpp = pcr.ifthenelse(
                ls.satDegUpp > 0.0, 
                lco.storUpp, 
                constrained_satDegUpp * lco.parameters.storCapUpp)
            # correct for any scaling issues (value < 0 or > 1 do not make sense
            lco.storUpp = pcr.min(1.0, pcr.max(0.0, lco.storUpp))
            lco.satDegUpp = vos.getValDivZero(
                  lco.storUpp, 
                  lco.parameters.storCapUpp,\
                  vos.smallNumber,0.)
            lco.satDegUpp = pcr.ifthen(
             lco.landmask, 
             lco.satDegUpp)
            lco.satDegUppTotal=lco.satDegUpp
            lco.storUppTotal=lco.storUpp

        # after updating we need to propagate the changes to the dependend variables
        # proper way to do this would be to implement a state model...(see AMUSE/OMUSE) 

        self.model.landSurface.storUpp=pcr.scalar(0.0)
        self.model.landSurface.satDegUpp=pcr.scalar(0.0)
        self.model.landSurface.storUppTotal=pcr.scalar(0.0)
        self.model.landSurface.satDegUppTotal=pcr.scalar(0.0)
        
        for coverType in ls.coverTypes:
            lco=ls.landCoverObj[coverType]
            land_cover_fraction = lco.fracVegCover
            land_cover_storUpp = lco.storUpp
            land_cover_satDegUpp = lco.satDegUpp
            land_cover_storUppTotal = lco.storUppTotal
            land_cover_satDegUppTotal = lco.satDegUppTotal
            self.model.landSurface.storUpp+= land_cover_fraction * land_cover_storUpp
            self.model.landSurface.satDegUpp+= land_cover_fraction * land_cover_satDegUpp
            self.model.landSurface.storUppTotal+= land_cover_fraction * land_cover_storUppTotal
            self.model.landSurface.satDegUppTotal+= land_cover_fraction * land_cover_satDegUppTotal
            # more needed?

        self.reporting.satDegUpp=self.model.landSurface.satDegUppTotal
        self.reporting.storUpp=self.model.landSurface.storUppTotal

    def set_precipitation(self,src):
        mask = np.isnan(src)
        src[mask] = 1e20
        precipitation = pcr.numpy2pcr(pcr.Scalar, src, 1e20)

        precipitation = pcr.ifthen(self.model.meteo.landmask, precipitation)
        #-----------------------------------------------------------------------

        # make sure that precipitation is always positive
        precipitation = pcr.max(0., precipitation)
        precipitation = pcr.cover(  precipitation, 0.0)
        
        # ignore very small values of precipitation (less than 0.00001 m/day or less than 0.01 kg.m-2.day-1 )
        if self.model.meteo.usingDailyTimeStepForcingData:
            precipitation = pcr.rounddown(precipitation*100000.)/100000.
        
        self.model.meteo.precipitation = precipitation

        self.reporting.precipitation  = pcr.ifthen(self.model.routing.landmask, self.model.meteo.precipitation)

        # update derived tbd

    def set_temperature(self,src):
        mask = np.isnan(src)
        src[mask] = 1e20
        temperature = pcr.numpy2pcr(pcr.Scalar, src, 1e20)

        temperature = pcr.ifthen(self.model.meteo.landmask, temperature)
        #-----------------------------------------------------------------------

        temperature = pcr.max(0., temperature)
        temperature = pcr.cover( temperature, 0.0)
                
        self.model.meteo.temperature = temperature

        self.reporting.temperature  = pcr.ifthen(self.model.routing.landmask, self.model.meteo.temperature)
        
    def set_channel_storage(self,src):
        mask = np.isnan(src)
        src[mask] = 1e20
        channel_storage = pcr.numpy2pcr(pcr.Scalar, src, 1e20)

        channel_storage = pcr.ifthen(self.model.meteo.landmask, channel_storage)
        #-----------------------------------------------------------------------

        channel_storage = pcr.max(0., channel_storage)
        channel_storage = pcr.cover( channel_storage, 0.0)
                
        self.model.routing.channelStorage = channel_storage

        self.reporting.channel_storage = pcr.ifthen(self.model.routing.landmask, self.model.routing.channelStorage)
        print(self.model.routing.channelStorage)
        print(1)
        print(src)
        print(channel_storage)

    def set_PotET(self,module,src):
        
        missing_value = module.missing_value
        
        ET = pcr.numpy2pcr(pcr.Scalar, src, missing_value)           
               
        module.impose_target_variable('totalPotET', ET, missing_value)
        
        
    def set_AE(self,module,src):
        
        missing_value = module.missing_value
        
        AE = pcr.numpy2pcr(pcr.Scalar, src, missing_value) 
                   
        module.impose_target_variable('actBareSoilEvap', AE, missing_value)
        

        
    def set_PE(self,module,src):
        
        missing_value = module.missing_value
        
        PE = pcr.numpy2pcr(pcr.Scalar, src, missing_value)           
               
        module.impose_target_variable('potBareSoilEvap', PE, missing_value)
                   
           
    def set_AT(self,module,src):
        
        missing_value = module.missing_value
        
        AT = pcr.numpy2pcr(pcr.Scalar, src, missing_value)           
               
        module.impose_target_variable('actTranspiration', AT, missing_value)
        
    
    def set_WE(self,module,src):
        
        missing_value = module.missing_value
        
        WE = pcr.numpy2pcr(pcr.Scalar, src, missing_value)           
               
        module.impose_target_variable('openWaterEvap', WE, missing_value)
        
    def set_Rootdepth(self,module,src):
        
        missing_value = module.missing_value
        
        RD = pcr.numpy2pcr(pcr.Scalar, src, missing_value)           
               
        module.impose_target_variable('root_depth', RD, missing_value)
        
         
    def set_value(self, module,long_var_name, src):

        if self.model is None:
            logger.info("cannot set value for %s, as model has not run yet.", long_var_name)
            return

        logger.info("setting value for %s", long_var_name)
        
        src = np.reshape(src, self.shape)
        
        logger.info("setting value shape %s", src.shape)
        
        # cast to pcraster precision
        src = src.astype(np.float32)

        # print "as float 32", src

        sys.stdout.flush()

        logger.info("setting value shape %s", src.shape)

        if long_var_name == "near_surface_soil_saturation_degree":
            self.set_satDegUpp000005(src)
        elif long_var_name == "upper_soil_saturation_degree":
            self.set_satDegUpp(src)
        elif long_var_name == "precipitation":
            self.set_precipitation(src)
        elif long_var_name == "temperature":
            self.set_temperature(src)
        elif long_var_name == "channel_storage":
            self.set_channel_storage(src)
        elif long_var_name == "totalPotET":
            self.set_PotET(module,src)   
        elif long_var_name == "actBareSoilEvap":
            self.set_AE(module,src)
        elif long_var_name == "potBareSoilEvap":
            self.set_PE(module,src)
        elif long_var_name == "actTranspiration":
            self.set_AT(module,src)
        elif long_var_name == "openWaterEvap":
            self.set_WE(module,src) 
        elif long_var_name == "root_depth":
            self.set_Rootdepth(module,src)
        else:
            raise Exception("unknown var name" + long_var_name)

        # write state here to facilitate restarting tomorrow
        # logger.info("dumping state to %s", self.configuration.endStateDir)
        # self.model.dumpStateDir(self.configuration.endStateDir + "/post/")

    def set_value_at_indices(self, long_var_name, indices, src):
        tmp=self.get_value(long_var_name)
        tmp.flat[indices]=src
        self.set_value(long_var_name, tmp)

    # grid_id is always 0
    def get_var_grid(self, long_var_name):
        return 0

    def get_grid_type(self, grid_id):
        return "uniform_rectilinear_grid"

    def get_grid_shape(self, grid_id):
        return self.shape

    def get_grid_spacing(self, grid_id):

        cellsize = pcr.clone().cellSize()

        return np.array([cellsize, cellsize])

    def get_grid_origin(self, grid_id):

        north = pcr.clone().north()
        cellSize = pcr.clone().cellSize()
        nrRows = pcr.clone().nrRows()

        south = north - (cellSize * nrRows)

        west = pcr.clone().west()

        return np.array([south, west])

    def get_grid_rank(self, grid_id):
        return len(self.shape)

    def get_grid_x(self, grid_id):
        north = pcr.clone().north()
        cellSize = pcr.clone().cellSize()
        nrRows = pcr.clone().nrRows()
        south = north - (cellSize * nrRows)
        spacing=pcr.clone().cellSize()
        return south+spacing*(np.arange(nrRows)+0.5)

    def get_grid_y(self, grid_id):
        west = pcr.clone().west()
        spacing=pcr.clone().cellSize()
        return west+spacing*(np.arange(self.shape[1])+0.5)

    def get_grid_z(self, grid_id):
        raise ValueError

    def get_grid_connectivity(self, long_var_name):
        raise ValueError

    def get_grid_offset(self, long_var_name):
        raise ValueError

    # EBMI functions

    def set_start_time(self, start_time):
        self.model_time.setStartTime(self.in_modeltime(start_time))

    def set_end_time(self, end_time):
        self.model_time.setEndTime(self.in_modeltime(end_time))

    def get_attribute_names(self):
        raise NotImplementedError

    def get_attribute_value(self, attribute_name):
        raise NotImplementedError

    def set_attribute_value(self, attribute_name, attribute_value):
        raise NotImplementedError

    def save_state(self, destination_directory):
        logger.info("saving state to %s", destination_directory)
        self.model.dumpStateDir(destination_directory)

    def load_state(self, source_directory):
        raise NotImplementedError


class ScaledBmiPCRGlobWB(BmiPCRGlobWB):
    factor = 5

    def set_value(self, long_var_name, scaled_new_value):
        # small value for comparison
        current_value = self.get_value(long_var_name)

        print ('current value after scaling', current_value)

        print ('value given by user', scaled_new_value)

        diff = scaled_new_value - current_value

        print ("diff now", diff)

        # scale to model resolution
        big_diff = np.repeat(np.repeat(diff, self.factor, axis=0), self.factor, axis=1)

        big_current_value = BmiPCRGlobWB.get_value(self, long_var_name)

        new_value = big_current_value + big_diff

        # new_value = np.repeat(np.repeat(src, self.factor, axis=0), self.factor, axis=1)

        BmiPCRGlobWB.set_value(self, long_var_name, new_value)

    def calculate_shape(self):
        original = BmiPCRGlobWB.calculate_shape(self)

        logger.info("original shape !!! =" + str(original))

        return np.array([original[0] // self.factor, original[1] // self.factor])

    def get_value(self, long_var_name):
        big_map = BmiPCRGlobWB.get_value(self, long_var_name)

        print ("getting value original shape " + str(big_map.shape))
        print ("original size " + str(big_map.size))
        print ("nans in original " + str(np.count_nonzero(np.isnan(big_map))))

        result = np.zeros(shape=self.get_grid_shape(long_var_name))

        if HAVE_DOWNSAMPLE:
            downsample(big_map, result)
        else:
            raise Exception("no downsample, cython compile imagemean")

        print ("getting value new shape " + str(result.shape))
        print ("result size " + str(result.size))
        print ("nans count in result " + str(np.count_nonzero(np.isnan(result))))

        print ("getting value", result)
        sys.stdout.flush()

        return result

    def get_grid_spacing(self, long_var_name):
        cellsize = pcr.clone().cellSize()
        
        return np.array([cellsize * self.factor, cellsize * self.factor])

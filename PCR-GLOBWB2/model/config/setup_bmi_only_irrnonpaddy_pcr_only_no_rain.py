[globalOptions]

# Please set the pcrglobwb output directory (outputDir) in an absolute path.
outputDir    = /scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/


# Please set the clone map file (cloneMap), which defines the spatial resolution and extent of your study area.
# - Please make sure that the file is stored locally in your computing machine.
# - The file must be in the pcraster format.
#~ cloneMap    = /data/hydroworld/others/RhineMeuse/RhineMeuse05min.clone.map
cloneMap    = /scratch/beek0120/mirca_land_cover_param/selected_catchment.map

# Set the input directory map in an absolute path. The input forcing and parameter directories and files will be relative to this.
# - The following is an example using files from the opendap server.
inputDir = /data/hydroworld/pcrglobwb2_input_release/version_2019_11_beta/pcrglobwb2_input/


# The area/landmask of interest:
# If None, area/landmask is limited for cells with ldd value.
landmask    = /scratch/beek0120/mirca_land_cover_param/selected_catchment.map


# netcdf attributes for output files:
institution = Department of Physical Geography, Utrecht University
title       = PCR-GLOBWB 2 output, only non-irrigated paddy, no other human water demand
description = PCR-GLOBWB run with irrigation only, no other human water demand, at 30 arc minutes (half degree)


startTime = 1991-01-01
endTime   = 1991-12-31
# Format: YYYY-MM-DD ; The model runs on daily time step.


# spinning up options:
maxSpinUpsInYears = 0
minConvForSoilSto = 0.0
minConvForGwatSto = 0.0
minConvForChanSto = 0.0
minConvForTotlSto = 0.0


[meteoOptions]

# Set the forcing temperature and precipitation files (relative to inputDir)
precipitationNC = /data/hydroworld/forcing/CRU-TS3.21/downcaled_with_ERA-Interim_1979_to_2010/cruts321_erainterim_daily_precipitation_1979_to_2010.nc
temperatureNC   = /data/hydroworld/forcing/CRU-TS3.21/downcaled_with_ERA-Interim_1979_to_2010/cruts321_erainterim_daily_temperature_1979_to_2010.nc

# bias the rainfall to check irrigtion using the conversion factors
precipitationConstant = 0.0
precipitationFactor   = 0.0

# Method to calculate referencePotETP (reference potential evaporation+transpiration)
# options are "Hamon" and "Input" ; If "Input", the netcdf input file must be given:
referenceETPotMethod = Input
refETPotFileNC  = /data/hydroworld/forcing/CRU-TS3.21/downcaled_with_ERA-Interim_1979_to_2010/cruts321_erainterim_daily_referencePotET_1979_to_2010.nc

# ~ [meteoDownscalingOptions]
# ~ # This section is for a 5 arcmin run, for downscaling meteorological forcing at 30 arcmin to 5 arcmin.
# this has been disabled here.


# ~ downscalePrecipitation  = False
# ~ downscaleTemperature    = True
# ~ downscaleReferenceETPot = False

# ~ # downscaling (based on the digital elevation model):
# ~ # The downscaling will be performed by providing the "cellIds" (meteoDownscaleIds) of lower resolution cells.
# ~ meteoDownscaleIds = None
# ~ highResolutionDEM = None

# ~ # lapse rates:
# ~ temperLapseRateNC =  None
# ~ precipLapseRateNC =  None

# ~ # downscaling criteria (TODO: remove these):
# ~ temperatCorrelNC  = input5min/meteo/downscalingFrom30ArcMin//temperature_correl.nc
# ~ precipitCorrelNC  = input5min/meteo/downscalingFrom30ArcMin//precipitation_correl.nc

# ~ # windows length (unit: arc-degree) for smoothing/averaging forcing data (not recommended):
# ~ smoothingWindowsLength = 0

[landSurfaceOptions]

debugWaterBalance = True

numberOfUpperSoilLayers = 2

# soil and parameters
# - they are used for all land cover types, unless they are are defined in certain land cover type options
#   (e.g. different/various soil types for agriculture areas)
topographyNC     = global_30min/landSurface/topography/topography_parameters_30_arcmin_october_2015.nc
soilPropertiesNC = global_30min/landSurface/soil/soilProperties.nc

includeIrrigation = True

# netcdf time series for historical expansion of irrigation areas (unit: hectares).
# Note: The resolution of this map must be consisten with the resolution of cellArea.
historicalIrrigationArea = None

# a pcraster map/value defining irrigation efficiency (dimensionless) - optional
# irrigation efficicency is set to unity - crop water requirements only
irrigationEfficiency     = 1.00

includeDomesticWaterDemand  = False
includeIndustryWaterDemand  = False
includeLivestockWaterDemand = False

# domestic, industrial and livestock water demand data (unit must be in m.day-1)
domesticWaterDemandFile  = None
industryWaterDemandFile  = None
livestockWaterDemandFile = None


# desalination water supply (maximum/potential/capacity)
desalinationWater = None

# zone IDs (scale) at which allocations of groundwater and surface water (as well as desalinated water) are performed  
allocationSegmentsForGroundSurfaceWater = global_30min/waterUse/abstraction_zones/abstraction_zones_60min_30min.nc

# pcraster maps defining the partitioning of groundwater - surface water source 
#
# IMPORTANT: all other options to limit change irrigation and non-irrigation water withdrawals have been removed
# and are included here for reference; NOTE: these options are excluded as prioritizing or restricting withdrawals
# is undesirable for testing purposes
#### - threshold values defining the preference for surface water source for irrigation purpose
#### -- treshold to maximize surface water irrigation use (cells with irrSurfaceWaterAbstractionFraction above this will prioritize irrigation surface water use)
###treshold_to_maximize_irrigation_surface_water           = 0.50
#### -- treshold to minimize fossil water withdrawal for irrigation (cells with irrSurfaceWaterAbstractionFraction below this have no fossil withdrawal for irrigation)
###treshold_to_minimize_fossil_groundwater_irrigation      = 0.70
####
#### - predefined surface water - groundwater partitioning for non irrigation demand (e.g. based on McDonald, 2014)
###maximumNonIrrigationSurfaceWaterAbstractionFractionData = global_30min/waterUse/source_partitioning/surface_water_fraction_for_non_irrigation/max_city_sw_fraction.nc

# original fractions are kept, it is expected that this will automatically change and tax non-renewable groundwater
# resources once surface water availability is limited

# - predefined surface water - groundwater partitioning for irrigation demand (e.g. based on Siebert, Global Map of Irrigation Areas version 5)
irrigationSurfaceWaterAbstractionFractionData           = global_30min/waterUse/source_partitioning/surface_water_fraction_for_irrigation/AEI_SWFRAC.nc
# -- quality map
irrigationSurfaceWaterAbstractionFractionDataQuality    = global_30min/waterUse/source_partitioning/surface_water_fraction_for_irrigation/AEI_QUAL.nc

# four standard land cover types are incuded. All vegetation fractions are set to zero
# except that of irrigated non-paddy for which the values for irrigated maize are used.

[forestOptions]

name = forest
debugWaterBalance = True

# snow module properties
snowModuleType      =  Simple
freezingT           =  0.0
degreeDayFactor     =  0.0025
snowWaterHoldingCap =  0.1
refreezingCoeff     =  0.05

# other paramater values
minTopWaterLayer  = 0.0
minCropKC         = 0.2

cropCoefficientNC   = global_30min/landSurface/landCover/naturalTall/Global_CropCoefficientKc-Forest_30min.nc
interceptCapNC    = global_30min/landSurface/landCover/naturalTall/interceptCapInputForest366days.nc
coverFractionNC   = global_30min/landSurface/landCover/naturalTall/coverFractionInputForest366days.nc

landCoverMapsNC   = None
# If NC file is not provided, we have to provide the following pcraster maps:
fracVegCover      = /scratch/beek0120/mirca_land_cover_param/veg_frac_dummy_empty.map
minSoilDepthFrac  = global_30min/landSurface/landCover/naturalTall/minf_tall_permafrost.map
maxSoilDepthFrac  = global_30min/landSurface/landCover/naturalTall/maxf_tall.map
rootFraction1     = /data/hydroworld/PCRGLOBWB20/input5min/landCover/forest/rfrac1_tall.map
rootFraction2     = /data/hydroworld/PCRGLOBWB20/input5min/landCover/forest/rfrac2_tall.map
maxRootDepth      = 1.0
# Note: The maxRootDepth is not used for non irrigated land cover type.
#

# initial conditions:
interceptStorIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interceptStor_forest_1999-12-31.nc
snowCoverSWEIni  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowCoverSWE_forest_1999-12-31.nc
snowFreeWaterIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowFreeWater_forest_1999-12-31.nc
topWaterLayerIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/topWaterLayer_forest_1999-12-31.nc
storUppIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storUpp_forest_1999-12-31.nc
storLowIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storLow_forest_1999-12-31.nc
interflowIni      = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interflow_forest_1999-12-31.nc


[grasslandOptions]

name = grassland
debugWaterBalance = True

# snow module properties
snowModuleType      =  Simple
freezingT           =  0.0
degreeDayFactor     =  0.0025
snowWaterHoldingCap =  0.1
refreezingCoeff     =  0.05

# other paramater values
minTopWaterLayer = 0.0
minCropKC        = 0.2

cropCoefficientNC   = global_30min/landSurface/landCover/naturalShort/Global_CropCoefficientKc-Grassland_30min.nc
interceptCapNC    = global_30min/landSurface/landCover/naturalShort/interceptCapInputGrassland366days.nc
coverFractionNC   = global_30min/landSurface/landCover/naturalShort/coverFractionInputGrassland366days.nc

landCoverMapsNC   = None
# If NC file is not provided, we have to provide the following values:
fracVegCover      = /scratch/beek0120/mirca_land_cover_param/veg_frac_dummy_empty.map
minSoilDepthFrac  = global_30min/landSurface/landCover/naturalShort/minf_short_permafrost.map
maxSoilDepthFrac  = global_30min/landSurface/landCover/naturalShort/maxf_short.map
rootFraction1     = global_30min/landSurface/landCover/naturalShort/rfrac1_short.map
rootFraction2     = global_30min/landSurface/landCover/naturalShort/rfrac2_short.map
maxRootDepth      = 0.5
# Note: The maxRootDepth is not used for non irrigated land cover type.
#

# initial conditions:
interceptStorIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interceptStor_grassland_1999-12-31.nc
snowCoverSWEIni  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowCoverSWE_grassland_1999-12-31.nc
snowFreeWaterIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowFreeWater_grassland_1999-12-31.nc
topWaterLayerIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/topWaterLayer_grassland_1999-12-31.nc
storUppIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storUpp_grassland_1999-12-31.nc
storLowIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storLow_grassland_1999-12-31.nc
interflowIni      = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interflow_grassland_1999-12-31.nc


[irrPaddyOptions]

name = irrPaddy
irrigationType = irrPaddy
debugWaterBalance = True

# snow module properties
snowModuleType      =  Simple
freezingT           =  0.0
degreeDayFactor     =  0.0025
snowWaterHoldingCap =  0.1
refreezingCoeff     =  0.05

#
# other paramater values
# NOTE: check the crop depletion factor
minTopWaterLayer = 0.05
minCropKC        = 0.2
cropDeplFactor   = 0.2
minInterceptCap  = 0.0002

cropCoefficientNC   = global_30min/landSurface/landCover/irrPaddy/Global_CropCoefficientKc-IrrPaddy_30min.nc

landCoverMapsNC   = None
# If NC file is not provided, we have to provide the following values:
fracVegCover      = /scratch/beek0120/mirca_land_cover_param/veg_frac_dummy_empty.map
minSoilDepthFrac = global_30min/landSurface/landCover/irrPaddy/minf_paddy_permafrost.map
maxSoilDepthFrac = global_30min/landSurface/landCover/irrPaddy/maxf_paddy.map
rootFraction1    = global_30min/landSurface/landCover/irrPaddy/rfrac1_paddy.map
rootFraction2    = global_30min/landSurface/landCover/irrPaddy/rfrac2_paddy.map
maxRootDepth     = 0.5

# initial conditions:
interceptStorIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interceptStor_irrPaddy_1999-12-31.nc
snowCoverSWEIni  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowCoverSWE_irrPaddy_1999-12-31.nc
snowFreeWaterIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowFreeWater_irrPaddy_1999-12-31.nc
topWaterLayerIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/topWaterLayer_irrPaddy_1999-12-31.nc
storUppIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storUpp_irrPaddy_1999-12-31.nc
storLowIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storLow_irrPaddy_1999-12-31.nc
interflowIni      = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interflow_irrPaddy_1999-12-31.nc


[irrNonPaddyOptions]

name = irrNonPaddy
irrigationType = irrNonPaddy
debugWaterBalance = True

# snow module properties
snowModuleType      =  Simple
freezingT           =  0.0
degreeDayFactor     =  0.0025
snowWaterHoldingCap =  0.1
refreezingCoeff     =  0.05

#
# other paramater values
minTopWaterLayer = 0.0
minCropKC        = 0.2
cropDeplFactor   = 0.5
minInterceptCap  = 0.0002

cropCoefficientNC   = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_cropCoefficientInput_kc_30arcmin.nc
interceptCapNC    = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_interceptionCapacity_interceptCapInput_30arcmin.nc
coverFractionNC   = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_coverFractionInput_30arcmin.nc   

landCoverMapsNC   = None
# If NC file is not provided, we have to provide the following values:
fracVegCover     = /scratch/beek0120/mirca_land_cover_param/veg_frac_dummy_full.map
minSoilDepthFrac = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_minSoilDepthFrac_30arcmin.map
maxSoilDepthFrac = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_maxSoilDepthFrac_30arcmin.map
rootFraction1    = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_rootFraction1_30arcmin.map
rootFraction2    = /scratch/beek0120/mirca_land_cover_param/irrigated_maize_rootFraction2_30arcmin.map
maxRootDepth     = 1.00
# Note: The maxRootDepth is not used for non irrigated land cover type.
#

# initial conditions:
interceptStorIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interceptStor_irrNonPaddy_1999-12-31.nc
snowCoverSWEIni  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowCoverSWE_irrNonPaddy_1999-12-31.nc
snowFreeWaterIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/snowFreeWater_irrNonPaddy_1999-12-31.nc
topWaterLayerIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/topWaterLayer_irrNonPaddy_1999-12-31.nc
storUppIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storUpp_irrNonPaddy_1999-12-31.nc
storLowIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storLow_irrNonPaddy_1999-12-31.nc
interflowIni      = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/interflow_irrNonPaddy_1999-12-31.nc

outDailyTotNC = storUppTotal,storLowTotal,actualET,actTranspiTotal,irrGrossDemand
outMonthTotNC = actualET,actTranspiTotal
outMonthAvgNC = storUppTotal,storLowTotal
outMonthEndNC = None

[groundwaterOptions]

debugWaterBalance = True

groundwaterPropertiesNC = global_30min/groundwater/properties/groundwaterProperties.nc
# The file will containspecificYield (m3.m-3), kSatAquifer (m.day-1), recessionCoeff (day-1)
#
# - minimum value for groundwater recession coefficient (day-1) 
minRecessionCoeff = 1.0e-4

# some options for constraining groundwater abstraction
# fossil groundwater abstraction should not be limited!
limitFossilGroundWaterAbstraction      = False
estimateOfRenewableGroundwaterCapacity = 0.0
estimateOfTotalGroundwaterThickness    = global_30min/groundwater/aquifer_thickness_estimate/thickness_30min.nc
# minimum and maximum total groundwater thickness 
minimumTotalGroundwaterThickness       = 100.
maximumTotalGroundwaterThickness       = None

# annual pumping capacity for each region (unit: billion cubic meter per year), should be given in a netcdf file
# this has been set to None, groundwater pumping in this case is not limiting.
pumpingCapacityNC = None

# these limits have been set unrealistically high to allow for nearly unlimited groundwater withdrawals
# (increased factor 100 compared to defaults); values are m/day
maximumDailyGroundwaterAbstraction      = 5.00
maximumDailyFossilGroundwaterAbstraction = 2.00

# initial conditions:
storGroundwaterIni                        = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storGroundwater_1999-12-31.nc
storGroundwaterFossilIni                  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/storGroundwaterFossil_1999-12-31.nc
#
# additional initial conditions for pumping behaviors
avgNonFossilGroundwaterAllocationLongIni  = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgNonFossilGroundwaterAllocationLong_1999-12-31.nc
avgNonFossilGroundwaterAllocationShortIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgNonFossilGroundwaterAllocationShort_1999-12-31.nc
avgTotalGroundwaterAbstractionIni         = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgTotalGroundwaterAbstraction_1999-12-31.nc
avgTotalGroundwaterAllocationLongIni      = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgTotalGroundwaterAllocationLong_1999-12-31.nc
avgTotalGroundwaterAllocationShortIni     = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgTotalGroundwaterAllocationShort_1999-12-31.nc
#
# additional initial conditions (needed only for MODFLOW run) 
# Not needed for this simulation
relativeGroundwaterHeadIni                = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/relativeGroundwaterHead_1999-12-31.nc
baseflowIni                                = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/baseflow_1999-12-31.nc

# zonal IDs (scale) at which zonal allocation of groundwater is performed  
allocationSegmentsForGroundwater = global_30min/waterUse/abstraction_zones/abstraction_zones_30min_30min.nc

[routingOptions]

debugWaterBalance = True

# drainage direction map
lddMap      = global_30min/routing/ldd_and_cell_area/lddsound_30min.nc

# cell area (unit: m2)
cellAreaMap = global_30min/routing/ldd_and_cell_area/cellarea30min.nc

# routing method:
routingMethod     = accuTravelTime
#~ routingMethod  = kinematicWave

# manning coefficient
manningsN   = 0.04

# Option for flood plain simulation
dynamicFloodPlain = True

# manning coefficient for floodplain
floodplainManningsN = 0.07


# channel gradient
gradient             = global_30min/routing/channel_properties/channel_gradient.nc

# constant channel depth 
constantChannelDepth = global_30min/routing/channel_properties/bankfull_depth.nc

# constant channel width (optional)
constantChannelWidth = global_30min/routing/channel_properties/bankfull_width.nc

# minimum channel width (optional)
minimumChannelWidth  = global_30min/routing/channel_properties/bankfull_width.nc

# channel properties for flooding
bankfullCapacity     = None
# - If None, it will be estimated from (bankfull) channel depth (m) and width (m) 


# files for relative elevation (above minimum dem) 
relativeElevationFiles  = global_30min/routing/channel_properties/dzRel%04d.nc
relativeElevationLevels = 0.0, 0.01, 0.05, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00


# composite crop factors for WaterBodies: 
cropCoefficientWaterNC = global_30min/routing/kc_surface_water/cropCoefficientForOpenWater.nc
minCropWaterKC         = 1.00


# lake and reservoir parameters
waterBodyInputNC       = global_30min/routing/surface_water_bodies/waterBodies30min.nc
onlyNaturalWaterBodies = False


# initial conditions:
waterBodyStorageIni            = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/waterBodyStorage_1999-12-31.nc
channelStorageIni              = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/channelStorage_1999-12-31.nc
readAvlChannelStorageIni       = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/readAvlChannelStorage_1999-12-31.nc
avgDischargeLongIni            = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgDischargeLong_1999-12-31.nc
avgDischargeShortIni           = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgDischargeShort_1999-12-31.nc
m2tDischargeLongIni            = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/m2tDischargeLong_1999-12-31.nc
avgBaseflowLongIni             = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgBaseflowLong_1999-12-31.nc
riverbedExchangeIni            = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/riverbedExchange_1999-12-31.nc
#
# initial condition of sub-time step discharge (needed for estimating number of time steps in kinematic wave methods)
subDischargeIni                = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/subDischarge_1999-12-31.nc
#
avgLakeReservoirInflowShortIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgLakeReservoirInflowShort_1999-12-31.nc
avgLakeReservoirOutflowLongIni = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/avgLakeReservoirOutflowLong_1999-12-31.nc
#
# number of days (timesteps) that have been performed for spinning up initial conditions in the routing module (i.e. channelStorageIni, avgDischargeLongIni, avgDischargeShortIni, etc.)
timestepsToAvgDischargeIni     = global_30min/initialConditions/non-natural/consistent_run_201903XX/1999/timestepsToAvgDischarge_1999-12-31.nc
# Note that: 
# - maximum number of days (timesteps) to calculate long term average flow values (default: 5 years = 5 * 365 days = 1825)
# - maximum number of days (timesteps) to calculate short term average values (default: 1 month = 1 * 30 days = 30)

[reportingOptions]

# output files that will be written in the disk in netcdf files:
# - daily resolution
outDailyTotNC = None
# - monthly resolution
outMonthTotNC = actualET,irrPaddyWaterWithdrawal,irrNonPaddyWaterWithdrawal,domesticWaterWithdrawal,industryWaterWithdrawal,livestockWaterWithdrawal,runoff,totalRunoff,baseflow,directRunoff,interflowTotal,totalGroundwaterAbstraction,desalinationAbstraction,surfaceWaterAbstraction,nonFossilGroundwaterAbstraction,fossilGroundwaterAbstraction,irrGrossDemand,nonIrrGrossDemand,totalGrossDemand,nonIrrWaterConsumption,nonIrrReturnFlow,precipitation,gwRecharge,surfaceWaterInf,referencePotET,totalEvaporation,totalPotentialEvaporation,totLandSurfaceActuaET,totalLandSurfacePotET,waterBodyActEvaporation,waterBodyPotEvaporation
outMonthAvgNC = discharge,temperature,dynamicFracWat,surfaceWaterStorage,interceptStor,snowFreeWater,snowCoverSWE,topWaterLayer,storUppTotal,storLowTotal,storGroundwater,storGroundwaterFossil,totalActiveStorageThickness,totalWaterStorageThickness,satDegUpp,satDegLow,channelStorage,waterBodyStorage
outMonthEndNC = storGroundwater,storGroundwaterFossil,waterBodyStorage,channelStorage,totalWaterStorageThickness,totalActiveStorageThickness
# - annual resolution
outAnnuaTotNC = totalEvaporation,precipitation,gwRecharge,totalRunoff,baseflow,desalinationAbstraction,surfaceWaterAbstraction,nonFossilGroundwaterAbstraction,fossilGroundwaterAbstraction,totalGroundwaterAbstraction,totalAbstraction,irrGrossDemand,nonIrrGrossDemand,totalGrossDemand,nonIrrWaterConsumption,nonIrrReturnFlow,runoff,actualET,irrPaddyWaterWithdrawal,irrNonPaddyWaterWithdrawal,irrigationWaterWithdrawal,domesticWaterWithdrawal,industryWaterWithdrawal,livestockWaterWithdrawal,precipitation_at_irrigation,netLqWaterToSoil_at_irrigation,evaporation_from_irrigation,transpiration_from_irrigation,referencePotET
outAnnuaAvgNC = temperature,discharge,surfaceWaterStorage,waterBodyStorage,interceptStor,snowFreeWater,snowCoverSWE,topWaterLayer,storUppTotal,storLowTotal,storGroundwater,storGroundwaterFossil,totalWaterStorageThickness,satDegUpp,satDegLow,channelStorage,waterBodyStorage,fractionWaterBodyEvaporation,fractionTotalEvaporation,fracSurfaceWaterAllocation,fracDesalinatedWaterAllocation,gwRecharge
outAnnuaEndNC = surfaceWaterStorage,interceptStor,snowFreeWater,snowCoverSWE,topWaterLayer,storUppTotal,storLowTotal,storGroundwater,storGroundwaterFossil,totalWaterStorageThickness
# - monthly and annual maxima
outMonthMaxNC = channelStorage,dynamicFracWat,floodVolume,floodDepth,surfaceWaterLevel,discharge,totalRunoff
outAnnuaMaxNC = None

# netcdf format and zlib setup
formatNetCDF = NETCDF4
zlib = True




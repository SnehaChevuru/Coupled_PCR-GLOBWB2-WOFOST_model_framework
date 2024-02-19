#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# PCR-GLOBWB (PCRaster Global Water Balance) Global Hydrological Model
#
# Copyright (C) 2016, Edwin H. Sutanudjaja, Rens van Beek, Niko Wanders, Yoshihide Wada,
# Joyce H. C. Bosmans, Niels Drost, Ruud J. van der Ent, Inge E. M. de Graaf, Jannis M. Hoch,
# Kor de Jong, Derek Karssenberg, Patricia López López, Stefanie Peßenteiner, Oliver Schmitz,
# Menno W. Straatsma, Ekkamol Vannametee, Dominik Wisser, and Marc F. P. Bierkens
# Faculty of Geosciences, Utrecht University, Utrecht, The Netherlands
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import os
import sys

import pcraster as pcr

from pcraster.framework import DynamicModel
from pcraster.framework import DynamicFramework

from model.configuration import Configuration
from model.currTimeStep import ModelTime
from model.reporting import Reporting
from model.spinUp import SpinUp

from model.pcrglobwb import PCRGlobWB

import logging
logger = logging.getLogger(__name__)

import oldcalc_framework
import disclaimer

test_bmi = False

class DeterministicRunner(DynamicModel):

    def __init__(self, configuration, modelTime, initialState = None):
        DynamicModel.__init__(self)

        self.modelTime = modelTime
        self.model = PCRGlobWB(configuration, modelTime, initialState)
        self.reporting = Reporting(configuration, self.model, modelTime)

    def initial(self):
        pass

    def dynamic(self):
        
        # test whether initial conditions of land cover are accessible or not
        coverType = 'irrNonPaddy'

        # re-calculate current model time using current pcraster timestep value
        self.modelTime.update(self.currentTimeStep())

        # this section is comprises the steps that are needed to initialize
        # the model prior to the full update by the self.model.update below
        # read in the meteo forcing
        self.model.read_forcings()
        # initialize any imposed fluxes per land cover type
        self.model.landSurface.initialize_imposed_landcover_fluxes()

        # test imposed land cover fluxes
        coverType = 'irrNonPaddy'

        if coverType in self.model.landSurface.landCoverObj.keys() and test_bmi:

            landCoverObj       = self.model.landSurface.landCoverObj[coverType]

            landmask           = landCoverObj.landmask
            imposed_flux_names = landCoverObj.imposed_flux_names
            missing_value      = landCoverObj.missing_value

            # 'totalPotET', 'potBareSoilEvap', \
            # 'actBareSoilEvap', \
            # 'actTranspiration', 'openWaterEvap'
            # AE is actual bare soil evaporation, AT is actual transpiration, ET is potential evapotranspiration, PE is potential evaporation and WE is open water evaporation. 

            imposed_fluxes = dict(zip(imposed_flux_names, \
                                      [0.005, 0.002, 0.001, 0.002, 0.0005]))

            for imposed_flux_name, imposed_flux_value in imposed_fluxes.items():

                logger.info('TEST: setting %20s to %6.4f for %s' % \
                            (imposed_flux_name, imposed_flux_value, coverType))

                landCoverObj.impose_target_flux(imposed_flux_name, \
                                                imposed_flux_value, \
                                                missing_value)

        # update model (will pick up current model time from model time object)
        # full update
        self.model.update(report_water_balance=True)


        landCoverObj       = self.model.landSurface.landCoverObj[coverType]

        # pcr.report(landCoverObj.topWaterLayer, os.path.join( \
        #           '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
        #           'maps', 'toplayer.%03d' % self.currentTimeStep()))

        # pcr.report(landCoverObj.infiltration, os.path.join( \
        #           '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
        #           'maps', 'infil000.%03d' % self.currentTimeStep()))

        # pcr.report(landCoverObj.storUpp, os.path.join( \
        #           '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
        #           'maps', 'storupp0.%03d' % self.currentTimeStep()))

        # pcr.report(landCoverObj.test_irrig_demand, os.path.join( \
        #           '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
        #           'maps', 'irr_est0.%03d' % self.currentTimeStep()))

        # pcr.report(landCoverObj.irrGrossDemand, os.path.join( \
        #           '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
        #           'maps', 'irr_dem0.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.root_depth, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'rootd000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.crop_stage_day_number, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'growth00.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.avgCropKC, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'kc000000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.growth_direction, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'gdir0000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.crop_day_number, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'day00000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.irrigation_probability, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'prob0000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.irrigation_water_requirement, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'irr_req0.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.irrGrossDemand, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'irr_dem0.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.theta, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'theta000.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.theta_fc, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'thetafc0.%03d' % self.currentTimeStep()))

        pcr.report(landCoverObj.theta_wp, os.path.join( \
                  '/scratch/beek0120/pcrglobwb2_output/test_bmi_30min_pcr_only_no_rain/', \
                  'maps', 'thetawp0.%03d' % self.currentTimeStep()))


    
        #do any needed reporting for this time step
        self.reporting.report()

def main():

    # print disclaimer
    disclaimer.print_disclaimer()

    # get the full path of configuration/ini file given in the system argument
    iniFileName   = os.path.abspath(sys.argv[1])

    # debug option
    debug_mode = False
    if len(sys.argv) > 2:
        if sys.argv[2] == "debug": debug_mode = True

    # no modification in the given ini file, use it as it is
    no_modification = True

    # use the output directory as given in the system argument
    if len(sys.argv) > 3 and sys.argv[3] == "--output_dir":
        no_modification = False
        output_directory = sys.argv[4]

    # object to handle configuration/ini file
    configuration = Configuration(iniFileName = iniFileName, \
                                  debug_mode = debug_mode, \
                                  no_modification = no_modification)
    if no_modification == False:
        configuration.main_output_directory = output_directory
        configuration.globalOptions['outputDir'] = output_directory
        configuration.set_configuration()

    # timeStep info: year, month, day, doy, hour, etc
    currTimeStep = ModelTime()

    # object for spin_up
    spin_up = SpinUp(configuration)

    # spinningUp
    noSpinUps = int(configuration.globalOptions['maxSpinUpsInYears'])
    initial_state = None
    if noSpinUps > 0:

        logger.info('Spin-Up #Total Years: '+str(noSpinUps))

        spinUpRun = 0 ; has_converged = False
        while spinUpRun < noSpinUps and has_converged == False:
            spinUpRun += 1
            currTimeStep.getStartEndTimeStepsForSpinUp(
                    configuration.globalOptions['startTime'],
                    spinUpRun, noSpinUps)
            logger.info('Spin-Up Run No. '+str(spinUpRun))
            deterministic_runner = DeterministicRunner(configuration, currTimeStep, initial_state)

            all_state_begin = deterministic_runner.model.getAllState()

            dynamic_framework = DynamicFramework(deterministic_runner,currTimeStep.nrOfTimeSteps)
            dynamic_framework.setQuiet(True)
            dynamic_framework.run()

            all_state_end = deterministic_runner.model.getAllState()

            has_converged = spin_up.checkConvergence(all_state_begin, all_state_end, spinUpRun, deterministic_runner.model.routing.cellArea)

            initial_state = deterministic_runner.model.getState()

    # Running the deterministic_runner (excluding DA scheme)
    currTimeStep.getStartEndTimeSteps(configuration.globalOptions['startTime'],
                                      configuration.globalOptions['endTime'])
    logger.info('Transient simulation run started.')
    deterministic_runner = DeterministicRunner(configuration, currTimeStep, initial_state)
    dynamic_framework = DynamicFramework(deterministic_runner,currTimeStep.nrOfTimeSteps)
    dynamic_framework.setQuiet(True)
    dynamic_framework.run()


    # for debugging to PCR-GLOBWB version one
    if configuration.debug_to_version_one:

        logger.info('\n\n\n\n\n'+'Executing PCR-GLOBWB version 1.'+'\n\n\n\n\n')

        # reset modelTime object
        currTimeStep = None; currTimeStep = ModelTime()
        currTimeStep.getStartEndTimeSteps(configuration.globalOptions['startTime'],
                                          configuration.globalOptions['endTime'])

        # execute PCR-GLOBWB version 1
        # - including comparing model outputs (from versions one and two)
        pcrglobwb_one = oldcalc_framework.PCRGlobWBVersionOne(configuration, \
                                                              currTimeStep, \
                                                              deterministic_runner.model.routing.landmask, \
                                                              deterministic_runner.model.routing.cellArea)
        dynamic_framework = DynamicFramework(pcrglobwb_one, currTimeStep.nrOfTimeSteps)
        dynamic_framework.setQuiet(True)
        dynamic_framework.run()

if __name__ == '__main__':
    # print disclaimer
    disclaimer.print_disclaimer(with_logger = True)
    sys.exit(main())

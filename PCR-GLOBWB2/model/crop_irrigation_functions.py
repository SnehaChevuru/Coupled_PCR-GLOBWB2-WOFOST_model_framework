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

import pcraster as pcr


"""
crop irrigation functions: this module contains functions that are intended to
compute the irrigation water requirements for paddy and non-paddy crops.
For non-paddy crops, the functions replicate the behaviour of FAO Drainage
Paper 56; for non-paddy crops, a slightly different approach is used, where
first saturation of the top soil and the formation of a water layer has to
be considered. In all cases, a first-time application of irrigation is sched-
uled that is dependent on the elapsed time the crop factor is above the min-
imum crop factor; the occurrence of irrigation increases with lower soil moist-
ure and the elapsed time.

In general:

    * the first day of irrigation occurrence is estimated starting from the
      first day the crop water coefficient is above a minimum value; if it is
      less than the minimum value, the estimate is reset to a default value;
      with the elapsed time that the crop factor exceeds the minimum value and
      the water stress factor, f, a running mean is computed of the estimate of
      the form p_irr(t) = 0.5 * (p_irr(t-1) + exp(-f / d))
      The first irrigation is then scheduled once p_irr > p_crit; default val-
      ues of p_crit are 0.9 and 0.8 for the initial value of p_irr(d = 0);
      with the default values, irrigation is scheduled within the first ten
      days.
    * the first irrigation amount is dependent on the type of irrigation;
      for non-paddy crops, the soil is brought to field capacity for the
      effective depth of evaporation or the root zone is brought to field
      capacity, whichever quanitity is the largest;
      for paddy crops, the first amount of irrigation is the amount of water
      required to bring the top soil to saturation over the effective depth
      of transpiration, increased by the saturated hydraulic conductivity and
      the initial depth of the ponded water layer.
    * subsequent irrigation amounts are computed per day within the growing
      season -given p_irr(t)- as the outstanding water demand;
    - for non-paddy crops this occurs if the soil moisture falls below RAW in
      the root zone, in which case the soil moisture is increased to TAW;
    - for paddy crops, a distinction is made between the ponded stage and the
      non-ponded stage. For both stages, a minimum irrigation water requirement
      is set on the basis of TAW and RAW as was the case for non-paddy irrigat-
      ed crops; this is the only irrigation water requirement in case of the
      non-ponded stage which occurs prior to harvesting and is schematized as
      the period when the crop factor falls below 0.75 for the first time and
      keeps decreasing; in the preceding ponded water stage, subsequent irrig-
      ation also occurs if the ponded water layer is less than the infiltrat-
      ion depth, which is taken equal to thesaturated hydraulic conductivty
      times the timestep length (day); in that case the ponded water layer is
      replenished to the required depth, and increased by the maximum of the
      expected infiltration depth or the irrigation water requirements for the
      crop.
    * for both paddy and non-paddy crops, the irrigation water requirements are
      corrected for the water that is available at the surface as a result of
      the rain. For PCR-GLOBWB2 this is the correct amount, for future applic-
      ations, it could be estimated using the rainfall and expeced irrigation
      water losses.

https://www.fao.org/3/x0490e/x0490e00.htm
https://www.fao.org/3/s2022e/s2022e08.htm

"""

def compute_total_saturation_depth(theta_sat, \
                                   theta, \
                                   effective_depth_evaporation):

    # return the total depth to saturation
    return (theta_sat - theta) * effective_depth_evaporation

def compute_total_evaporable_water(theta_fc, \
                                  theta_wp, \
                                  effective_depth_evaporation):

    # return the total evaporable water
    return (theta_fc - 0.5 * theta_wp) * effective_depth_evaporation


def compute_total_available_water(theta_fc, \
                                  theta_wp, \
                                  root_depth):

    # return total available water
    return (theta_fc - theta_wp) * root_depth


def compute_readily_available_water(theta, \
                                  theta_wp, \
                                  root_depth):

    # return the readily available water
    return (theta - theta_wp) * root_depth


def adjust_depletion_factor_for_texture(depletion_factor, \
                                        se_50, \
                                        se_fc, \
                                        se_wp, \
                                        limit_p_min = 0.10, \
                                        limit_p_max = 0.80, \
                                        p_adjustment = 0.20):

    # return the crop depletion factor adjusted for soil texture
    ratio = (2 * se_50 - se_fc - se_wp) / (se_fc - se_wp)
    return pcr.min(pcr.scalar(limit_p_max),
                   pcr.max(pcr.scalar(limit_p_min),
                   depletion_factor + p_adjustment * ratio))

def adjust_depletion_factor_for_ETc(depletion_factor,
                                    crop_potential_evapotranspiration, \
                                    limit_p_min  = 0.10, \
                                    limit_p_max  = 0.80, \
                                    ETc_ref      = 0.005, \
                                    p_adjustment = 0.04):

    # return the crop depletion factor adjusted for the crop-specific
    # potential evapotranspiratio rate
    return pcr.min(pcr.scalar(limit_p_max),
                   pcr.max(pcr.scalar(limit_p_min),
                   depletion_factor + p_adjustment * \
                   (ETc_ref - crop_potential_evapotranspiration)))


def estimate_paddy_infiltration_capacity(ksat,
        min_infiltration_capacity = 0.006, \
        max_infiltration_capacity = 0.008):

    # estimates the reduced infiltration capacity on paddy rice fields due to
    # puddling; the reduced infiltration capacity is scaled between
    # the minimum and maximum values that were based on the FAO guidelines
    # of Brouwer and Heibloem (1986); otherwise, the saturated hydraulic cond-
    # uctivity is reduced by a factor of 10 similar to Wada et al. (2014).
    # all values are in metres per day

    return pcr.min(pcr.scalar(max_infiltration_capacity), \
                   pcr.max(pcr.scalar(min_infiltration_capacity), 0.10 * ksat))

def get_irrigation_water_requirement( \
        irrigation_type, \
        min_water_layer_depth, \
        depletion_factor, \
        irrigation_efficiency, \
        effective_depth_evaporation, \
        root_depth, \
        percolation_loss, \
        paddy_infiltration_capacity, \
        potential_evaporation, \
        open_water_evaporation, \
        liquid_water_input, \
        water_layer_depth, \
        theta_sat, \
        theta_res, \
        theta_fc, \
        theta_wp, \
        theta, \
        allow_irrigation, \
        early_irrigation, \
        paddy_irrigation, \
        paddy_irrigation_type = 'irrPaddy'):

    # compute the relevant irrigation parameters
    adjusted_depletion_factor = adjust_depletion_factor_for_ETc(depletion_factor, \
                                       potential_evaporation)

    total_saturation_depth  = compute_total_saturation_depth(theta_sat, theta, \
                                                             effective_depth_evaporation)
    total_saturation_depth  = pcr.max(0, total_saturation_depth)
    total_evaporable_water  = compute_total_evaporable_water(theta_fc, theta_wp, \
                                                             effective_depth_evaporation)
    total_evaporable_water  = pcr.max(0, total_evaporable_water)
    total_available_water   = compute_total_available_water(theta_fc, theta_wp, \
                                                             root_depth)
    total_available_water   =    pcr.max(0, total_available_water)
    readily_available_water = compute_readily_available_water(theta, theta_wp, \
                                                             root_depth)
    readily_available_water = pcr.max(0, readily_available_water)

    # default FAO irrigation
    irrigation_water_requirement = pcr.ifthenelse(readily_available_water < \
                                                  adjusted_depletion_factor * \
                                                  total_available_water, \
                                                  pcr.max(0, total_available_water - \
                                                              readily_available_water) + \
                                                  percolation_loss, 0)
    irrigation_water_requirement = pcr.ifthenelse(early_irrigation, \
                                                  pcr.max((theta_fc - theta) * \
                                                      effective_depth_evaporation + \
                                                      percolation_loss, \
                                                      irrigation_water_requirement), \
                                                  irrigation_water_requirement)

    if irrigation_type == paddy_irrigation_type:

        irrigation_water_requirement = pcr.ifthenelse(paddy_irrigation, \
                                           pcr.max(0, min_water_layer_depth - \
                                                   water_layer_depth) + \
                                           paddy_infiltration_capacity + \
                                           open_water_evaporation + \
                                           pcr.ifthenelse(early_irrigation, \
                                               total_saturation_depth, 0), \
                                           irrigation_water_requirement )

    # is there irrigation and is it needed? and what is the efficiency?
    irrigation_water_requirement = pcr.max(0, irrigation_water_requirement - \
                                              liquid_water_input)
    irrigation_water_requirement = irrigation_water_requirement / irrigation_efficiency

    irrigation_water_requirement = pcr.ifthenelse(allow_irrigation, \
                                       irrigation_water_requirement, 0)
        
    # return irrigation water requirement
    return irrigation_water_requirement
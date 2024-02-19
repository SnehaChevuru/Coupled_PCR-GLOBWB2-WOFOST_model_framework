cmake_minimum_required(VERSION 3.5)

set (WOFOST_VERSION 2.0)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/wofostMaggyc.pc.cmake
                ${CMAKE_BINARY_DIR}/WOFOST_Maggy/wofostMaggyc.pc)	


include(GNUInstallDirs)

set (sources Afgen.c Assim.c Astro.c Clean.c Conversion.c copy_dmy.c Cropdata.c CropNutrientRates.c Develop.c DM-Partioning.c Evtra.c Fillvar.c Functions.c GetMeteoInput.c GetSimInput.c Growth.c IfSowing.c InitializeCrop.c InitializeNutrients.c IntegrationCrop.c IntegrationNutrients.c Lai.c LeaveGrowth.c Leaves.c List.c Management.c Meteodata.c Moment.c NutrientLoss.c NutrientPartitioning.c Nutrients.c NutrientTranslocation.c NutritionIndex.c Output.c Penman.c RateCalculationCrop.c RatesToZero.c Respiration.c SimData.c Sitedata.c Soildata.c SoilNutrientRates.c Watfd.c wofostSupCode.c wofost_copy_library.c)

if(WIN32)
	add_library(bmiwofostMaggyc ${sources})
else()
	add_library(bmiwofostMaggyc SHARED ${sources})
endif()

#target_include_directories(bmiwofostc PUBLIC
#	$<BUILd_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}
#	$<INSTALL_INTERFACE:include>
#	PRIVATE ${sources})

target_link_libraries(bmiwofostMaggyc -lm -lnetcdf)
########### wofost main ###############


#set (wofost_sources Afgen.c Assim.c Astro.c Clean.c Conversion.c copy_dmy.c Cropdata.c CropNutrientRates.c Develop.c DM-Partioning.c Evtra.c Fillvar.c Functions.c GetMeteoInput.c GetSimInput.c Growth.c IfSowing.c InitializeCrop.c InitializeNutrients.c IntegrationCrop.c IntegrationNutrients.c Lai.c LeaveGrowth.c Leaves.c List.c Management.c Meteodata.c Moment.c NutrientLoss.c NutrientPartitioning.c Nutrients.c NutrientTranslocation.c NutritionIndex.c Output.c Penman.c RateCalculationCrop.c RatesToZero.c Respiration.c SimData.c Sitedata.c Soildata.c SoilNutrientRates.c Watfd.c Wofost.c wofost_copy_library.c)

#add_executable (run_cwofost ${wofost_sources})
#target_link_libraries(run_cwofost -lm -lnetcdf)

set (wofostbmi_sources Afgen.c Assim.c Astro.c Clean.c Conversion.c copy_dmy.c Cropdata.c CropNutrientRates.c Develop.c DM-Partioning.c Evtra.c Fillvar.c Functions.c GetMeteoInput.c GetSimInput.c Growth.c IfSowing.c InitializeCrop.c InitializeNutrients.c IntegrationCrop.c IntegrationNutrients.c Lai.c LeaveGrowth.c Leaves.c List.c Management.c Meteodata.c Moment.c NutrientLoss.c NutrientPartitioning.c Nutrients.c NutrientTranslocation.c NutritionIndex.c Output.c Penman.c RateCalculationCrop.c RatesToZero.c Respiration.c SimData.c Sitedata.c Soildata.c SoilNutrientRates.c Watfd.c wofostSupCode.c wofost_bmi.c wofost_copy_library.c)

add_executable (run_bmiwofostMaggyc ${wofostbmi_sources})
target_link_libraries(run_bmiwofostMaggyc -lm -lnetcdf)
	
	

########### Install files ###############

install(TARGETS run_bmiwofostMaggyc
  RUNTIME DESTINATION bin
  COMPONENT WOFOST_Maggy)
install(TARGETS bmiwofostMaggyc
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib 
	RUNTIME DESTINATION bin
	COMPONENT WOFOST_Maggy)
install(FILES wofost_bmi.h wofost.h
        DESTINATION include
        COMPONENT WOFOST_Maggy)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wofostMaggyc.pc
        DESTINATION lib/pkgconfig)

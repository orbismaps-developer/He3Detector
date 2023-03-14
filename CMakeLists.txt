cmake_minimum_required(VERSION 3.24)
project(He3Detector)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


set(CMAKE_CXX_STANDARD 17)

add_executable(He3Detector main.cpp src/He3GasMix.cpp include/PrimaryGeneratorAction.h include/SteppingVerbose.h include/RunManager.h include/StackingAction.h include/SaveAndResetEvent.h src/SteppingVerbose.cc src/StackingAction.cc src/SaveAndResetEvent.cpp include/detector.h src/detector.cpp src/TextDetectorBuilder.cpp include/TextDetectorBuilder.h include/LineProcessor.h src/LineProcessor.cpp include/Generator.h include/EventAction.h)
target_link_libraries(He3Detector ${Geant4_LIBRARIES})


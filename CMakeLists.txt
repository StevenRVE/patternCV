cmake_minimum_required(VERSION 3.7)
 project(sve-patternCV)

 add_subdirectory(dpf)

 dpf_add_plugin(sve-patternCV
   TARGETS clap lv2 vst2 vst3

   FILES_DSP
       source/patternCV.cpp
       source/patternGenerator.cpp
       source/euclideanGenerator.cpp
 )

 target_include_directories(sve-patternCV
   PUBLIC source)

 target_compile_options(sve-patternCV PUBLIC "-Wno-c++11-narrowing")

 add_compile_options(-fdiagnostics-color=always)
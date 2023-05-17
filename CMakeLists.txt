cmake_minimum_required(VERSION 3.7)
 project(sve-patternCV)

 add_subdirectory(dpf)

 dpf_add_plugin(sve-patternCV
   TARGETS lv2 vst2 vst3

   FILES_DSP
       source/patternCV.cpp
       source/patternGenerator.cpp
       source/euclideanGenerator.cpp
 )

 target_include_directories(sve-patternCV
   PUBLIC source)

 target_compile_options(sve-patternCV PUBLIC "-Wno-c++11-narrowing")

 add_compile_options(-fdiagnostics-color=always)

 set(CMAKE_RUNTIME_OUTPUT_LV2_DIRECTORY ${CMAKE_BINARY_DIR}/bin/sve-patternCV.lv2)
 set(CMAKE_RUNTIME_OUTPUT_VST3_DIRECTORY ${CMAKE_BINARY_DIR}/bin/sve-patternCV.vst3)
 set(CMAKE_RUNTIME_OUTPUT_VST2_DLL ${CMAKE_BINARY_DIR}/bin/sve-patternCV-vst2.dll)
 set(LV2_DIRECTORY C:/program files/LV2)
 set(VST3_DIRECTORY C:/program files/Common Files/VST3)
 set(VST2_DIRECTORY C:/program files/VSTPlugins)

 # Create a custom command to copy the directory
 add_custom_command(
         TARGET sve-patternCV
         POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_LV2_DIRECTORY} ${LV2_DIRECTORY}
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_VST3_DIRECTORY} ${VST3_DIRECTORY}
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_VST2_DLL} ${VST2_DIRECTORY}
 )

# Create a custom target to ensure the directory copy is performed after building
add_custom_target(copy_directory_target DEPENDS sve-patternCV)
add_dependencies(sve-patternCV copy_directory_target)
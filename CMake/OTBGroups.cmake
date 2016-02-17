# Set a list of group names
set(group_list
  Adapters
  Applications
  Core
  Detection
  Feature
  Filtering
  Fusion
  Hyperspectral
  IO
  Learning
  OBIA
  Radiometry
  Registration
  Remote
  Segmentation
  ThirdParty
  Visualization
  Wrappers
  )

set(Adapters_documentation "This group contains adapters class to third party software")
set(Applications_documentation "This group contains the applications shipped with Orfeo TooLBox")
set(Core_documentation "This group contains the core module used in Orfeo ToolBox")
set(Detection_documentation "This group contains algorithms related to detection of low or high level objects")
set(Feature_documentation "This group contains algorithms related to the computation of features")
set(Filtering_documentation "This group contains algorithms for classical image to image processing")
set(Fusion_documentation "This group contains algorithms for data fusion, including pan-sharpening")
set(Hyperspectral_documentation "This group contains algorithms dedicated to hyperspectral remote sensing")
set(IO_documentation "This group contains everything related to input/output")
set(Learning_documentation "This group contains algorithms and frameworks related to supervised or unsupervised learning")
set(OBIA_documentation "This group contains algorithms related to Object Based Image Analysis")
set(Radiometry_documentation "This group contains algorithms related to the processing of image radiometry")
set(Registration_documentation "This group contains algorithms related to registration of images")
set(Segmentation_documentation "This group contains algorithms related to image segmentaiton")
set(ThirdParty_documentation "This group contains all Orfeo ToolBox third parties")
set(Visualization_documentation "This group contains Ice (visualization framework) and IceViewer (visualization tool)")
set(Wrappers_documentation "This group contains the application framework and the wrappers to use it")


set(Remote_documentation "This group of modules is for OTB based code that have
additional third-party dependencies not bundled with the toolkit,
new algorithms or implementations seeking greater exposure and adoption,
algorithms that hope to eventually be integrated into the toolkit,
niche algorithms with limited application, and Modules in progress that do not
yet have the test coverage and cross-platform standards required by the main toolkit.
The modules are OFF by default in ITK's CMake configuration.
Note that these modules do get the same level of support and backwards
compatibility as other modules in the toolkit.")

#------------------------------------------------
# Find the modules in each group and the module name line in otb-module.cmake
foreach( group ${group_list} )
  set( _${group}_module_list )
  file( GLOB_RECURSE _${group}_module_files ${OTB_SOURCE_DIR}/Modules/${group}/otb-module.cmake )
  foreach( _module_file ${_${group}_module_files} )
    file( STRINGS ${_module_file} _module_line REGEX "otb_module[ \n]*\\([ \n]*[A-Za-z0-9]*" )
    string( REGEX MATCH "(\\([ \n]*)([A-Za-z0-9]*)" _module_name ${_module_line} )
    set( _module_name ${CMAKE_MATCH_2} )
    set( _${_module_name}_module_line ${_module_line} )
    list( APPEND _${group}_module_list ${_module_name} )
  endforeach()
endforeach()

#------------------------------------------------
# Set up Doxygen Group descriptions

set( group_list_dox )
foreach(group ${group_list} )
  set( group_list_dox
"${group_list_dox}
// -----------------------------------------------
// Group ${group}
/** \\defgroup Group-${group} Group ${group}
${${group}_documentation} */\n"
    )

  foreach(mod ${_${group}_module_list} )
    set( group_list_dox
"${group_list_dox}
/** \\defgroup ${mod} Module ${mod}
\\ingroup Group-${group} */\n"
      )
  endforeach()
endforeach()

set( _content ${group_list_dox} )
configure_file(
  "${OTB_SOURCE_DIR}/Utilities/Doxygen/Module.dox.in"
  "${OTB_BINARY_DIR}/Utilities/Doxygen/Modules/OTB-AllGroups.dox"
  )

#------------------------------------------------
# Turn on the OTB_BUILD option for each group

# Set a module name list for each group and exclude
# Modules that should be OFF
foreach( group ${group_list} )
  set( _${group}_on_module_list )
  list( LENGTH _${group}_module_list _num_modules )
  set( _current_module 0 )
  while( ${_current_module} LESS ${_num_modules} )
    list( GET _${group}_module_list ${_current_module} _module_name )
    if( NOT OTB_MODULE_${_module_name}_EXCLUDE_FROM_DEFAULT )
      list( APPEND _${group}_on_module_list ${_module_name} )
    endif()
  math( EXPR _current_module "${_current_module} + 1" )
  endwhile()
endforeach()

if("$ENV{DASHBOARD_TEST_FROM_CTEST}" STREQUAL "")
  # developer build
  option(OTBGroup_Core "Request building core modules" ON)
endif()
foreach( group ${group_list})
    option(OTBGroup_${group} "Request building ${group} modules" OFF)
    if (OTBGroup_${group})
      foreach (otb-module ${_${group}_on_module_list} )
         list(APPEND OTB_MODULE_${otb-module}_REQUEST_BY OTBGroup_${group})
      endforeach()
    endif()
    # Hide group options if building all modules anyway.
    if(OTB_BUILD_DEFAULT_MODULES)
      set_property(CACHE OTBGroup_${group} PROPERTY TYPE INTERNAL)
    else()
      set_property(CACHE OTBGroup_${group} PROPERTY TYPE BOOL)
    endif()
endforeach()

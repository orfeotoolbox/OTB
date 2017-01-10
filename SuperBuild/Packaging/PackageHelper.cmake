macro(macro_super_package)
  cmake_parse_arguments(PKG  "" "STAGE_DIR" "" ${ARGN} )
 
  set(LINUX FALSE)
  if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux" AND NOT MINGW)
    set(LINUX TRUE)
  endif()
    
  if("${PKG_STAGE_DIR}" STREQUAL "")
    message(FATAL_ERROR "PKG_STAGE_DIR is empty. Just can't continue.")
  endif()

  if(NOT DEPENDENCIES_INSTALL_DIR)
    message(FATAL_ERROR "DEPENDENCIES_INSTALL_DIR is not set of empty")
  endif()

  if(LINUX)
    if(NOT FILE_COMMAND)
      message(FATAL_ERROR "warning: No 'file' command, cannot continue...")
    endif()

    if(NOT PATCHELF_PROGRAM)
      message(FATAL_ERROR "PATCHELF_PROGRAM not set")
    endif()
  endif(LINUX)
  
  #setting this variable. prints a lot of debug information
  #set( PKG_DEBUG 1)

  set(loader_program_PATHS)
  set(eol_char "E")
  if(MSVC)
    set(loader_program_PATHS)
    set(loader_program_names      "dumpbin")
    set(loader_program_args       "/DEPENDENTS")
    set(loader_program_regex      "^    ([^ ].*[Dd][Ll][Ll])${eol_char}$")
  elseif(APPLE)
    set(loader_program_PATHS)
    set(loader_program_names    otool)
    set(loader_program_args      "-l" )
    set(loader_program_regex ".*name.*([ ].*dylib ).*${eol_char}$")
  elseif(LINUX)
    set(loader_program_PATHS)
    set(loader_program_names    objdump)
    set(loader_program_args    "-p" )
    set(loader_program_regex "^..NEEDED.*([ ].*so.*)${eol_char}$")
  elseif(MINGW)
    set(loader_program_PATHS    "${MXE_MXEROOT}/usr/bin")
    set(loader_program_names    "${OTB_TARGET_SYSTEM_ARCH}-w64-mingw32.shared-objdump")
    set(loader_program_args     "-p")
    set(loader_program_regex    "^\t*DLL Name: (.*\\.[Dd][Ll][Ll])${eol_char}$")
  else()
    message(FATAL_ERROR "Unknown platform: ")
  endif()
  
  find_program(LOADER_PROGRAM "${loader_program_names}" PATHS ${loader_program_PATHS})
  if(NOT LOADER_PROGRAM)
    message(FATAL_ERROR "${loader_program_names} not found in ${loader_program_PATHS}.")
  endif()

  include(GetPrerequisites)

  set(LOADER_PROGRAM_ARGS ${loader_program_args})

  set(DEST_BIN_DIR bin)
  set(DEST_APP_DIR lib/otb/applications)
  
  set(LIB_PREFIX lib)
  set(DEST_LIB_DIR lib)
  set(EXE_EXT "")
  set(SCR_EXT ".sh")
  set(LIB_EXT ".so")
  set(PYMODULE_EXT ".so")
  if(WIN32)
    set(LIB_PREFIX)
    set(DEST_LIB_DIR bin)
    set(EXE_EXT ".exe")
    set(LIB_EXT ".dll")
    set(SCR_EXT ".bat")
    set(PYMODULE_EXT ".pyd")
  elseif(APPLE)
    set(LIB_EXT ".dylib")
  endif()

  # find_loader_and_args(LOADER_PROGRAM LOADER_PROGRAM_ARGS)
  
  find_python_soname(python_INSTALLED_SONAME)

  set(PKG_SEARCHDIRS)
  if(WIN32)
    if(MSVC)
      list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/bin") #all other dlls
      list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/lib") #Qt & Qwt dlls

      if(NOT PKG_GENERATE_XDK)
	if(DEFINED ENV{UniversalCRTSdkDir})
          file(TO_CMAKE_PATH "$ENV{UniversalCRTSdkDir}" UCRT_SDK_DIR)
          list(
            APPEND
            PKG_SEARCHDIRS
            "${UCRT_SDK_DIR}/Redist/ucrt/DLLs/${OTB_TARGET_SYSTEM_ARCH}"
            ) #ucrt dlls
	else()
          message(FATAL_ERROR
	    "UniversalCRTSdkDir variable not set. call vcvarsall.bat <arch> first before starting build.")
	endif()
	
	#addtional msvc redist dll from VCINSTALLDIR
	if(DEFINED ENV{VCINSTALLDIR})
          file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" PKG_VCINSTALLDIR)
          list(
            APPEND
            PKG_SEARCHDIRS
            "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.CRT"
            "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.OPENMP"
            )
	else()
          message(FATAL_ERROR
	    "VCINSTALLDIR variable not set. call vcvarsall.bat <arch> first before starting build.")
	endif()
      endif() #NOT PKG_GENERATE_XDK
    else()
      file(GLOB MXE_GCC_LIB_DIR "${DEPENDENCIES_INSTALL_DIR}/bin/gcc*")
      list(APPEND PKG_SEARCHDIRS ${MXE_GCC_LIB_DIR})
      list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/qt/bin") #Qt
      list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/qt/lib") #Qwt
      list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/bin") #mxe dlls
    endif()
  else() #unixes
    list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/lib") #so
    list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/lib") #superbuild .so /.dylib
  endif()

  #common for all platforms.
  set(OTB_APPLICATIONS_DIR "${OTB_INSTALL_DIR}/lib/otb/applications")
  list(APPEND PKG_SEARCHDIRS "${DEPENDENCIES_INSTALL_DIR}/bin") #superbuild, mxe binaries
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/bin") #otbApplicationLauncherCommandLine..
  list(APPEND PKG_SEARCHDIRS "${OTB_APPLICATIONS_DIR}") #otb apps
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/lib/otb/python") #otbApplication.py

  if(PKG_GENERATE_XDK)
   list(APPEND PKG_SEARCHDIRS ${OTB_BINARY_DIR}/bin)
  endif()

  macro_empty_package_staging_directory()

  set(PKG_PEFILES)

  func_prepare_package()

  set(program_list)
  set(WITH_PYTHON "false")
  if(OTB_WRAP_PYTHON)
    set(WITH_PYTHON "true")
  endif()
  
  set(IS_XDK "false")

  if(LINUX)
    set(PKGSETUP_IN_FILENAME linux_pkgsetup.in)
  elseif(APPLE)
    set(PKGSETUP_IN_FILENAME macx_pkgsetup.in)
  endif()

  if(PKG_GENERATE_XDK)
    set(IS_XDK "true")
    set(WITH_PYTHON "false")
    message(STATUS "OTB_WRAP_PYTHON is set. But this will not be included in XDK")
    if("${PKG_ITK_SB_VERSION}" STREQUAL "")
      message(FATAL_ERROR "PKG_ITK_SB_VERSION not set. This is required for XDK")
    endif()

    func_install_xdk_files()
  endif() #PKG_GENERATE_XDK
    
  ############# install package configure script ################
  #if(UNIX AND NOT WIN32)
  if(UNIX AND NOT WIN32)    
    #avoid OTB stuff inside make_symlinks script
    file(STRINGS "${CMAKE_BINARY_DIR}/make_symlinks_temp" make_symlinks_list)
#    func_lisp( make_symlinks_list )
    file(WRITE ${CMAKE_BINARY_DIR}/make_symlinks "#!/bin/sh\n")
    foreach(make_symlink_cmd ${make_symlinks_list})
      file(APPEND ${CMAKE_BINARY_DIR}/make_symlinks
        "${make_symlink_cmd}\n")
    endforeach()
    
    configure_file(${PACKAGE_SUPPORT_FILES_DIR}/${PKGSETUP_IN_FILENAME}
      ${CMAKE_BINARY_DIR}/pkgsetup @ONLY)
    
    list(APPEND program_list "${CMAKE_BINARY_DIR}/pkgsetup")
    list(APPEND program_list "${CMAKE_BINARY_DIR}/make_symlinks")
 
    ########### install patchelf( linux only) ##################
    if(LINUX)
      list(APPEND program_list "${PATCHELF_PROGRAM}")
    endif()
      
  endif()


  foreach(prog ${program_list})
    install(
      PROGRAMS ${prog}
      DESTINATION ${PKG_STAGE_DIR})
  endforeach()


  # We need qt.conf on windows. for macx and linux we write it
  # after extracting package
  if(WIN32 AND NOT PKG_GENERATE_XDK)
    install(FILES
      ${PACKAGE_SUPPORT_FILES_DIR}/qt.conf
      DESTINATION ${PKG_STAGE_DIR}/bin
      )
  endif()

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README
    DESTINATION ${PKG_STAGE_DIR})

endmacro(macro_super_package)


function(func_prepare_package)



  file(WRITE ${CMAKE_BINARY_DIR}/make_symlinks_temp  "")

  #This must exist in any OTB Installation. minimal or full
  #set(PKG_PEFILES "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
  if(NOT EXISTS "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
    message(
      FATAL_ERROR
      "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT} not found.")
  endif()
  
  set(PKG_PEFILES "otbApplicationLauncherCommandLine${EXE_EXT}")

  foreach(exe_file
      "otbApplicationLauncherQt" "iceViewer" "otbTestDriver" "monteverdi" "mapla")
    if(EXISTS "${OTB_INSTALL_DIR}/bin/${exe_file}${EXE_EXT}")
        list(APPEND PKG_PEFILES "${exe_file}${EXE_EXT}")
    else()
      message(STATUS "${exe_file}${EXE_EXT} not found in ${OTB_INSTALL_DIR}/bin. (skipping)")
    endif()
  endforeach()
  
  if(PKG_GENERATE_XDK)
    #Qt stuff
    list(APPEND PKG_PEFILES "lrelease${EXE_EXT}")
    list(APPEND PKG_PEFILES "moc${EXE_EXT}")
    list(APPEND PKG_PEFILES "qmake${EXE_EXT}")
    list(APPEND PKG_PEFILES "rcc${EXE_EXT}")
    list(APPEND PKG_PEFILES "uic${EXE_EXT}")
    list(APPEND PKG_PEFILES "proj${EXE_EXT}")
    list(APPEND PKG_PEFILES "cs2cs${EXE_EXT}")

    #shark is optional
    if(EXISTS "${DEPENDENCIES_INSTALL_DIR}/bin/sharkVersion${EXE_EXT}")
      list(APPEND PKG_PEFILES "sharkVersion${EXE_EXT}")
    endif()
    
    #RK: there is a bug in itk cmake files in install tree 
    #we workaround with below code
    #start hack
     file(GLOB itk_all_lib_files  
     "${DEPENDENCIES_INSTALL_DIR}/${DEST_LIB_DIR}/${LIB_PREFIX}itk*${LIB_EXT}*"
     "${DEPENDENCIES_INSTALL_DIR}/${DEST_LIB_DIR}/${LIB_PREFIX}ITK*${LIB_EXT}*"
     )

   foreach(itk_lib_file ${itk_all_lib_files})
     func_is_file_a_symbolic_link("${itk_lib_file}" a_symlink itk_lib_file_target)
     if(NOT a_symlink)
       list(APPEND PKG_PEFILES "${itk_lib_file}")
     endif()
     endforeach()
     #end hack

    file(GLOB otb_test_exe_list 
    "${DEPENDENCIES_INSTALL_DIR}/bin/gdal*${EXE_EXT}"    
    "${OTB_BINARY_DIR}/bin/*Test*${EXE_EXT}"
    )
    foreach(otb_test_exe   ${otb_test_exe_list})
      get_filename_component(otb_test_exe_name ${otb_test_exe} NAME)
      list(APPEND PKG_PEFILES ${otb_test_exe_name})
    endforeach()
  endif()

  # special case for msvc: ucrtbase.dll must be explicitly vetted.
  if(MSVC AND NOT PKG_GENERATE_XDK)
    list(APPEND PKG_PEFILES "ucrtbase.dll")
  endif()
  
  file(GLOB OTB_APPS_LIST "${OTB_APPLICATIONS_DIR}/otbapp_*${LIB_EXT}") # /lib/otb
  list(APPEND PKG_PEFILES ${OTB_APPS_LIST})

  if(NOT PKG_GENERATE_XDK)
    if(EXISTS "${OTB_INSTALL_DIR}/lib/otb/python/_otbApplication${PYMODULE_EXT}")
      install(
	DIRECTORY
	${OTB_INSTALL_DIR}/lib/otb/python
	DESTINATION ${PKG_STAGE_DIR}/lib
	)
    else()
      if(OTB_WRAP_PYTHON)
	message(FATAL_ERROR "OTB_WRAP_PYTHON is set , but cannot find _otbApplication${PYMODULE_EXT}")
      endif()
    endif()   
  endif()#  if(NOT PKG_GENERATE_XDK)


  func_install_support_files()

  unset(matched_vars CACHE)
  get_vars_ending_with("_USED|_RESOLVED" matched_vars)
  foreach (var_to_unset IN LISTS matched_vars)
    if(PKG_DEBUG)
      message("unset ${var_to_unset} from cache")
    endif()
    unset(${var_to_unset} CACHE)
  endforeach()

  foreach(infile ${PKG_PEFILES})
   get_filename_component(bn ${infile} NAME)
   func_process_deps(${bn})
  endforeach()
 
endfunction() #func_prepare_package

function(func_process_deps input_file)

  search_library(${input_file} PKG_SEARCHDIRS input_file_full_path)
  if(NOT input_file_full_path)
    if(LINUX)
      setif_value_in_list(is_gtk_lib "${input_file}" ALLOWED_SYSTEM_DLLS)
      if(is_gtk_lib)
	search_library(${input_file} PKG_GTK_SEARCHDIRS input_file_full_path)
	if( NOT input_file_full_path)
	  message(FATAL_ERROR "${input_file} not found. searched in ${PKG_GTK_SEARCHDIRS}")
	endif()
      endif()
      if( NOT input_file_full_path)
	message(FATAL_ERROR "${input_file} not found. searched in ${PKG_SEARCHDIRS}")
	endif()
    endif(LINUX)

  endif() #if(NOT input_file_full_path)

  message("Processing ${input_file_full_path}")

  set(is_executable FALSE)
  is_file_executable2("${input_file_full_path}" is_executable)
  if(NOT is_executable)
    pkg_install_rule(${input_file_full_path})
    message("not is_executable ${input_file_full_path}")
    return()
  endif() #NOT is_executable

  if(UNIX)
    # Deal with symlinks.
    # For any valid symlinks, (see 'not_valid' below) 
    # we append ln -s source target commands to a file
    # That file is  executed during installation. 
    get_filename_component(bn_we ${input_file_full_path} NAME_WE)
    get_filename_component(bn_path ${input_file_full_path} PATH)
    
    file(GLOB sofiles "${bn_path}/${bn_we}*")
    foreach(sofile ${sofiles})
      get_filename_component(basename_of_sofile ${sofile} NAME)
      get_filename_component(sofile_ext ${sofile} EXT)
      set(not_valid FALSE)
      if(  "${sofile_ext}" MATCHES ".la"
          OR "${sofile_ext}" MATCHES ".prl"
          OR "${sofile_ext}" MATCHES ".a"
          OR  IS_DIRECTORY "${sofile}" )
        set(not_valid TRUE)
      endif()
      
      if(not_valid)
        continue()
      endif()
      
      func_is_file_a_symbolic_link("${sofile}" is_symlink linked_to_file)
      
      if(is_symlink)
        add_to_symlink_list("${linked_to_file}" "${basename_of_sofile}")	
      endif() # is_symlink
      
    endforeach()
    
  endif(UNIX)
  
  set(raw_items)
     
  execute_process(
    COMMAND ${LOADER_PROGRAM} ${LOADER_PROGRAM_ARGS} "${input_file_full_path}"
    RESULT_VARIABLE loader_rv
    OUTPUT_VARIABLE loader_ov
    ERROR_VARIABLE loader_ev
    )
  
  if(loader_rv)
    message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
  endif()

  string(REPLACE ";" "\\;" candidates "${loader_ov}")
  string(REPLACE "\n" "${eol_char};" candidates "${candidates}")
  
  get_filename_component(bn_name ${input_file_full_path} NAME)
  set(${bn_name}_USED TRUE CACHE INTERNAL "")

  foreach(candidate ${candidates})
    if(NOT candidate)
      continue()
    endif()
    if(NOT "${candidate}" MATCHES "${loader_program_regex}")
      continue()
    endif()
   
    string(REGEX REPLACE "${loader_program_regex}" "\\1" raw_item "${candidate}")

    if(NOT raw_item)
      continue()
    endif()  
      
    string(STRIP ${raw_item} raw_item)
    set(is_system FALSE)
    setif_value_in_list(is_system "${raw_item}" SYSTEM_DLLS)

    if(APPLE AND NOT is_system)
      if("${raw_item}" MATCHES "@rpath")
	string(REGEX REPLACE "@rpath." "" raw_item "${raw_item}")
      else()
	message(FATAL_ERROR "'${raw_item}' does not have @rpath")
      endif()
    endif()
    
    if(is_system  OR ${raw_item}_RESOLVED OR ${raw_item}_USED)
      continue()        
    endif()
   
    if(PKG_DEBUG)
      message("${raw_item} is not resolved, used. ${raw_item}_RESOLVED | ${raw_item}_USED")
    endif()
    
    list(APPEND raw_items ${raw_item})
    
  endforeach()

  if(PKG_DEBUG)
    string(REPLACE ";" "\n" raw_items_pretty_print "${raw_items}")
    message(FATAL_ERROR "raw_items=${raw_items_pretty_print}")
  endif(PKG_DEBUG)

  if(raw_items)
    list(REVERSE raw_items)
    foreach(item ${raw_items})      
      search_library(${item} PKG_SEARCHDIRS item_full_path)
      set(is_a_symlink FALSE)
      set(item_target_file)
      if(PKG_DEBUG)
	message("item0=${item_full_path}")
      endif()
      func_is_file_a_symbolic_link("${item_full_path}" is_a_symlink item_target_file)      
      if(is_a_symlink)
	set(${item}_RESOLVED TRUE CACHE INTERNAL "")
	set(item ${item_target_file})
      endif()
      if(PKG_DEBUG)
	message("running func_process_deps on '${item}'")
      endif()
      func_process_deps(${item})
  endforeach()
endif()

set(${bn_name}_RESOLVED TRUE CACHE INTERNAL "")

pkg_install_rule(${input_file_full_path})


endfunction() #function(func_process_deps infile)

function(pkg_install_rule src_file)

  get_filename_component(src_file_EXT ${src_file} EXT)
  get_filename_component(src_file_NAME ${src_file} NAME)

  set(file_type PROGRAMS)
  if(NOT src_file_EXT)
    set(output_dir "bin")  
  elseif("${src_file_EXT}" MATCHES "(.[Dd][Ll][Ll]|.exe)")
    set(output_dir "bin")
  elseif("${src_file_EXT}" MATCHES "(.lib|.so|.dylib)")
    set(output_dir "lib")
    set(file_type FILES)
  else()
    message(FATAL_ERROR "unreachable code")
  endif()
  
  set(SKIP_INSTALL FALSE)
  setif_value_in_list(is_gtk_lib "${src_file_NAME}" ALLOWED_SYSTEM_DLLS)  
  if(is_gtk_lib)
    set(output_dir "lib/gtk")
     set(SKIP_INSTALL TRUE)
  endif()
  
  #special case
  if("${src_file_NAME}" MATCHES "^otbapp_")
    set(output_dir "lib/otb/applications")
    set(file_type PROGRAMS)
  endif()

  if(PKG_GENERATE_XDK)
    if ("${src_file_NAME}"
	MATCHES
	"([Oo][Tt][Bb])|([Mm]onteverdi)|mapla|iceViewer"
	)
      set(SKIP_INSTALL TRUE)
        
      message("SKIP_INSTALL for ${src_file_NAME}")
    endif()

  endif()

  if(NOT SKIP_INSTALL)
    install(${file_type}
      "${src_file}"
      DESTINATION
      "${PKG_STAGE_DIR}/${output_dir}")
  endif()
  
endfunction()   


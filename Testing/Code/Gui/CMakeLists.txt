
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(GUI_TESTS ${CXX_TEST_PATH}/otbGuiTests)

# -------            otb::System  -------------
add_test(guTuFltkFilterWatcher ${GUI_TESTS}
        otbFltkFilterWatcher
        ${INPUTDATA}/couleurs.tif
        )


# -------------  otb::FltkWriterWatcher ----------------------------

add_test(coTvFltkWriterWatcher ${GUI_TESTS}
     otbFltkWriterWatcher
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/coTvFltkWriterWatcherOutput.tif
        20)



# -------       Fichiers sources CXX -----------------------------------
set(Gui_SRCS
otbGuiTests.cxx
otbFltkFilterWatcher.cxx
otbFltkWriterWatcher.cxx
)

# ${TIFF_LIBRARY}

# To suppress "ld: cycle in dylib re-exports with /usr/X11R6/lib/libGL.dylib" error on APPLE configuration
if(APPLE)
      foreach(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        set(CMAKE_EXE_LINKER_FLAGS${c} "${CMAKE_EXE_LINKER_FLAGS${c}} -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
      endforeach(c)
endif(APPLE)

OTB_ADD_EXECUTABLE(otbGuiTests "${Gui_SRCS}" "OTBIO;OTBGui;OTBTesting")

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

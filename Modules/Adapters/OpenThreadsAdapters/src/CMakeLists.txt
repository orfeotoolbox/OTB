set(OTBOpenThreadsAdapters_SRC
  otbThreads.cxx
  )

add_library(OTBOpenThreadsAdapters ${OTBOpenThreadsAdapters_SRC})
target_link_libraries(OTBOpenThreadsAdapters 
 ${OTBOpenThreads_LIBRARIES}
  )

otb_module_target(OTBOpenThreadsAdapters)

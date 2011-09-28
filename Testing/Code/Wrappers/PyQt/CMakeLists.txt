
foreach(app ${OTB_APPLICATIONS_NAME_LIST})

add_test(NAME pqTvShow_${app}
         COMMAND otbTestDriver
              --add-before-env PYTHONPATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG"
              --add-before-env PYTHONPATH        $<TARGET_FILE_DIR:_otbApplication>
              --add-before-env PYTHONPATH        "${CMAKE_SOURCE_DIR}/Code/Wrappers/PyQt"
              --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_${app}>
              Execute ${PYTHON_EXECUTABLE} 
              ${CMAKE_CURRENT_SOURCE_DIR}/PyQtTest.py
              ${app})
       
endforeach(app)
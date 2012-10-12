
OTB_CREATE_APPLICATION(NAME           RadiometricVegetationIndices
                       SOURCES        otbRadiometricVegetationIndices.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)

OTB_CREATE_APPLICATION(NAME           RadiometricWaterIndices
                       SOURCES        otbRadiometricWaterIndices.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)
                       
OTB_CREATE_APPLICATION(NAME           RadiometricIndices
                       SOURCES        otbRadiometricIndices.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)

OTB_CREATE_APPLICATION(NAME           OpticalCalibration
                       SOURCES        otbOpticalCalibration.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)

OTB_CREATE_APPLICATION(NAME           SarRadiometricCalibration
                       SOURCES        otbSarRadiometricCalibration.cxx
                       LINK_LIBRARIES OTBIO;OTBRadiometry)

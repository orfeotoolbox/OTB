Update of ITK in OTB source tree
================================

# Remove old version
find Utilities/ITK -type f -not -name "README.OTB.txt" | xargs rm
find Utilities/ITK/* -type d | xargs rm -r

# Copy new version
cp -R /home/jmalik/dev/src/ITK/* Utilities/ITK
rm -rf Utilities/ITK/.git*

# Remove ITK module test code
find Utilities/ITK -type d | grep -v vxl_shared_link_test | grep test$ | xargs rm -rf

# Remove ThirdParty code we don't need
rm -rf Utilities/ITK/Modules/ThirdParty/DCMTK
rm -rf Utilities/ITK/Modules/ThirdParty/DICOMParser
rm -rf Utilities/ITK/Modules/ThirdParty/GDCM
rm -rf Utilities/ITK/Modules/ThirdParty/GIFTI
rm -rf Utilities/ITK/Modules/ThirdParty/HDF5
rm -rf Utilities/ITK/Modules/ThirdParty/JPEG
rm -rf Utilities/ITK/Modules/ThirdParty/MINC
rm -rf Utilities/ITK/Modules/ThirdParty/NIFTI
rm -rf Utilities/ITK/Modules/ThirdParty/NrrdIO
rm -rf Utilities/ITK/Modules/ThirdParty/OpenJPEG
rm -rf Utilities/ITK/Modules/ThirdParty/PNG
rm -rf Utilities/ITK/Modules/ThirdParty/TIFF

# Remove IO modules code we don't need 
find Utilities/ITK/Modules/IO  -maxdepth 1 -mindepth 1 -type d | grep -v ImageBase | xargs rm -rf

rm -rf Utilities/ITK/Modules/Core/TestKernel
rm -rf Utilities/ITK/Modules/Bridge
rm -rf Utilities/ITK/Modules/Numerics/FEM
rm -rf Utilities/ITK/Modules/Nonunit/Review
rm -rf Utilities/ITK/Modules/Segmentation/LevelSetsv4Visualization
rm -rf Utilities/ITK/Modules/Registration/FEM
rm -rf Utilities/ITK/Modules/Compatibility/Deprecated


# Remaining ThirdParty :
DoubleConversion (needed by Common)
Expat (needed by MetaIO)
KWSys (needed by Common)
MetaIO (needed by SpatialObjects)
Netlib (needed by Statistics)
VNL (needed by VNLInstantiation Netlib)
VNLInstantiation (needed by Common)
ZLIB (needed by MetaIO)



# Remove directory we don't use in OTB
rm -rf Utilities/ITK/Documentation
rm -rf Utilities/ITK/Examples
rm -rf Utilities/ITK/Testing
rm -rf Utilities/ITK/Utilities/Dart
rm -rf Utilities/ITK/Utilities/DevelopmentSetupScripts
rm -rf Utilities/ITK/Utilities/Git
rm -rf Utilities/ITK/Utilities/Hooks
rm -rf Utilities/ITK/Utilities/InstallTest
rm     Utilities/ITK/Utilities/SetupForDevelopment.sh





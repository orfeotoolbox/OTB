#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

#----------- KmzExport TESTS ----------------
otb_test_application(NAME apTvUtKmzExport
                     APP KmzExport
                     OPTIONS -in   ${INPUTDATA}/qb_RoadExtract.img
                             -out ${TEMP}/apTvUtKmzExport.kmz
                             -logo ${INPUTDATA}/cnes.png
                             -legend ${INPUTDATA}/otb_big.png
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory)
set_property(TEST apTvUtKmzExport PROPERTY RESOURCE_LOCK kmz_tmp_dir)

otb_test_application(NAME apTvUtKmzExport1Band
                     APP KmzExport
                     OPTIONS #-in /media/otbnas/public/Data/Saterre/data/cumulLAI.tif
                             -in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                             -out ${TEMP}/apTvUtKmzExport1Band.kmz
                             -logo ${INPUTDATA}/cnes.png
                             -legend ${INPUTDATA}/otb_big.png
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -tilesize 50) # needed to see the bug mantis #773
set_property(TEST apTvUtKmzExport1Band PROPERTY RESOURCE_LOCK kmz_tmp_dir)

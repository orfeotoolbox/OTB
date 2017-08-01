#
# Copyright (C) 2017 CS SI
#
# This file is part of remote module MatchingFilters
#
#     TODO
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

set(OTBStereoFiltersTests
otbStereoFiltersTestDriver.cxx
cvf.cpp
PatchMatch.cpp
)


add_executable(otbStereoFiltersTestDriver ${OTBStereoFiltersTests})
target_link_libraries(otbStereoFiltersTestDriver ${OTBStereoFilters-Test_LIBRARIES})
otb_module_target_label(otbStereoFiltersTestDriver)

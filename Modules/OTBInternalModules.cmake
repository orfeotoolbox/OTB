#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

# create list for module name, desc, repo url and tag
# INDEX OF LISTS MUST BE COHERENT!!!
# When a new internal module is added, the four lists; otb_internal_groups, 
# otb_internal_groups_repos, otb_internal_groups_git_tag,
# otb_internal_groups_desc
# /!\ BEWARE OF THIS LIST ORDER
# As cmake read modules in this order if you break it may have cmake
# configuration problem. Note that
# - FeaturesExtraction depends of Core
# - Learning depends of FeaturesExtraction, Core
# - SAR depends of FeaturesExtraction, Core
# - Segmentation depends of FeaturesExtraction, Core
# - Hyperspectral depends of FeaturesExtraction, Core
# - StereoProcessing depends of FeaturesExtraction, Core
# - Miscellaneous depends of Learning, FeaturesExtraction, Core
list(APPEND otb_internal_groups
    "FeaturesExtraction"
    "Learning"
    "SAR"
    "Segmentation"
    "Hyperspectral"
    "StereoProcessing"
    "Miscellaneous")

list(APPEND otb_internal_groups_repos
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/featuresextraction.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/learning.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/sar.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/segmentation.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/hyperspectral.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/stereoprocessing.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/miscellaneous.git"
)

list(APPEND otb_internal_groups_git_tag
    "cleanup_P0" # FeaturesExtraction
    "cleanup_P0" # Learning
    "cleanup_P0" # SAR
    "cleanup_P0" # Segmentation
    "cleanup_P0" # Hyperspectral
    "cleanup_P0" # StereoProcessing
    "cleanup_P0" # Miscellaneous
    )

list(APPEND otb_internal_groups_get_submodules
    "OFF" # FeaturesExtraction
    "OFF" # Learning
    "OFF" # SAR
    "OFF" # Segmentation
    "OFF" # Hyperspectral
    "OFF" # StereoProcessing
    "OFF" # Miscellaneous 
    )

list(APPEND otb_internal_groups_desc
"This module contains classical filtering applications, such as texture extraction, edge extraction, smoothing, morphological operations. This module relies on external libraries (MuParser / MuParserX) contained in OTB-Dependencies. it can be installed along with the Core package with a simple tar extract command and directly available after sourcing the otbenv.profile." # FeaturesExtraction

"This module contains Machine Learning applications based on classical supervised or unsupervised algorithms (SVM, Random Forest, K-Means, Multi-layer Perceptron Neural Network, etc.). Its applications handle the whole processing chain : sample selection, learning, prediction and finalization of a classification map. It also contains a regression framework." # Learning

"This module contains SAR (Synthetic Aperture Radar) applications and covers all the scope of SAR imagery manipulation : SAR Calibration, Unspeckle with temporal filtering, Interferogram, etc." # SAR

"This group contains algorithms related to image segmentation" # Segmentation

"This module contains specific applications to handle hyperspectral images : spectral angle classification, end member number estimation or unmixing." # Hyperspectral

"This module allows to register stereoscopic images and compute disparity in order to build a 3D model from a couple of satellite images in raw / sensor geometry." #StereoProcessing

"This module deals with image simulation algorithm. Using
objects transmittance and reflectance and sensor characteristics, it can be possible
to generate realistic hyperspectral synthetic set of data. This module includes
PROSPECT (leaf optical properties) and SAIL (canopy bidirectional reflectance)
models, as well as PROSAIL, which is the combination of the two previous ones." # Miscellaneous
)

set(__i 0) # group index
list(LENGTH otb_internal_groups __nb_internal_groups)

# Check if arrays are coherents
list(LENGTH otb_internal_groups_repos __nb_urls)
list(LENGTH otb_internal_groups_git_tag __nb_branches)
list(LENGTH otb_internal_groups_get_submodules __nb_submodule_settings)
list(LENGTH otb_internal_groups_desc __nb_docs_desc)

if (NOT (${__nb_internal_groups} EQUAL ${__nb_urls}))
    message(FATAL_ERROR "There is one P0 remote module name or module url missings, aborting")
endif()
if (NOT (${__nb_internal_groups} EQUAL ${__nb_branches}))
    message(FATAL_ERROR "There is one P0 remote module name or module branches missing, aborting")
endif()
if (NOT (${__nb_internal_groups} EQUAL ${__nb_submodule_settings}))
    message(FATAL_ERROR "There is one P0 remote module name or submodule setting missing, aborting")
endif()
if (NOT (${__nb_internal_groups} EQUAL ${__nb_docs_desc}))
    message(FATAL_ERROR "There is one P0 remote module name or module doc description missing, aborting. You can add an empty description with \"\"")
endif()
unset(__nb_urls)
unset(__nb_branches)
unset(__nb_submodule_settings)
unset(__nb_docs_desc)

# NOTE TLA: a list of module dependency should be described here.
# to avoid cmake problems

# Now arrays are clean

# for all enabled internal module,
while(${__i} LESS ${__nb_internal_groups})
    list(GET otb_internal_groups "${__i}" __otb_group_name)
    # download only enabled modules
    if (OTBGroup_${__otb_group_name} OR OTB_BUILD_${__otb_group_name})
        # Following variable is mandatory for otb_fetch compat
        set(Module_${__otb_group_name} ON)
        set(__location "${OTB_SOURCE_DIR}/Modules/${__otb_group_name}")
        list(GET otb_internal_groups_repos "${__i}" __otb_group_repo)
        list(GET otb_internal_groups_git_tag "${__i}" __otb_group_tag)
        list(GET otb_internal_groups_desc "${__i}" __otb_group_desc)
        list(GET otb_internal_groups_get_submodules "${__i}" __get_submodules)

        message(STATUS "Downloading internal module ${__otb_group_name} at ${__location} with ref ${__otb_group_tag} and submodules at ${__get_submodules}")
        otb_fetch_module("${__otb_group_name}"
                         "${__otb_group_desc}"
                         ${__location}
                         GIT_REPOSITORY "${__otb_group_repo}"
                         GIT_TAG "${__otb_group_tag}"
                         GIT_SUBMODULES "${__get_submodules}"
        )
        set_property(GLOBAL APPEND PROPERTY ENABLED_GROUPS ${__otb_group_name})
        add_subdirectory("${__location}")

    endif() # ${OTBGroup_${module_name}}
    math(EXPR __i "${__i}+1")
endwhile() # i LESS_EQUAL __nb_internal_groups

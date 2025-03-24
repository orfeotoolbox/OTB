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
# When a new internal module is added, the four lists; otb_internal_modules, 
# otb_internal_modules_repos, otb_internal_modules_git_tag,
# otb_internal_modules_desc
list(APPEND otb_internal_modules
    "Miscellaneous"
    "FeaturesExtraction"
    "Learning")

list(APPEND otb_internal_modules_repos
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/miscellaneous.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/featuresextraction.git"
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/learning.git")

list(APPEND otb_internal_modules_git_tag
    "main" # Miscellaneous
    "main" # FeaturesExtraction
    "init_P0_module" # Learning
    )

list(APPEND otb_internal_modules_get_submodules
     "OFF" # Miscellaneous 
     "OFF" # FeaturesExtraction
     "OFF" # Learning
    )

list(APPEND otb_internal_modules_desc
"This module deals with image simulation algorithm. Using
objects transmittance and reflectance and sensor characteristics, it can be possible
to generate realistic hyperspectral synthetic set of data. This module includes
PROSPECT (leaf optical properties) and SAIL (canopy bidirectional reflectance)
models, as well as PROSAIL, which is the combination of the two previous ones." # Miscellaneous

"This module contains classical filtering applications, such as texture extraction, edge extraction, smoothing, morphological operations. This module relies on external libraries (MuParser / MuParserX) contained in OTB-Dependencies. it can be installed along with the Core package with a simple tar extract command and directly available after sourcing the otbenv.profile." # FeaturesExtraction

"This module contains Machine Learning applications based on classical supervised or unsupervised algorithms (SVM, Random Forest, K-Means, Multi-layer Perceptron Neural Network, etc.). Its applications handle the whole processing chain : sample selection, learning, prediction and finalization of a classification map. It also contains a regression framework." # Learning
)

set(__i 0) # group index
list(LENGTH otb_internal_modules __nb_internal_modules)

# Check if arrays are coherents
list(LENGTH otb_internal_modules_repos __nb_urls)
list(LENGTH otb_internal_modules_git_tag __nb_branches)
list(LENGTH otb_internal_modules_get_submodules __nb_submodule_settings)
list(LENGTH otb_internal_modules_desc __nb_docs_desc)

if (NOT (${__nb_internal_modules} EQUAL ${__nb_urls}))
    message(FATAL_ERROR "There is one P0 remote module name or module url missings, aborting")
endif()
if (NOT (${__nb_internal_modules} EQUAL ${__nb_branches}))
    message(FATAL_ERROR "There is one P0 remote module name or module branches missing, aborting")
endif()
if (NOT (${__nb_internal_modules} EQUAL ${__nb_submodule_settings}))
    message(FATAL_ERROR "There is one P0 remote module name or submodule setting missing, aborting")
endif()
if (NOT (${__nb_internal_modules} EQUAL ${__nb_docs_desc}))
    message(FATAL_ERROR "There is one P0 remote module name or module doc description missing, aborting. You can add an empty description with \"\"")
endif()
unset(__nb_urls)
unset(__nb_branches)
unset(__nb_submodule_settings)
unset(__nb_docs_desc)

# Now arrays are clean

# for all enabled internal module, 
while(${__i} LESS ${__nb_internal_modules})
    list(GET otb_internal_modules "${__i}" __otb_module_name)
    # download only enabled modules
    if (OTBGroup_${__otb_module_name} OR OTB_BUILD_${__otb_module_name})
        # Following variable is mandatory for otb_fetch compat
        set(Module_${__otb_module_name} ON)
        set(__location "${OTB_SOURCE_DIR}/Modules/${__otb_module_name}")
        list(GET otb_internal_modules_repos "${__i}" __otb_module_repo)
        list(GET otb_internal_modules_git_tag "${__i}" __otb_module_tag)
        list(GET otb_internal_modules_desc "${__i}" __otb_module_desc)
        list(GET otb_internal_modules_get_submodules "${__i}" __get_submodules)

        message(STATUS "Downloading internal module ${__otb_module_name} at ${__location} with submodules at ${__get_submodules}")
        otb_fetch_module("${__otb_module_name}"
                         "${__otb_module_desc}"
                         ${__location}
                         GIT_REPOSITORY "${__otb_module_repo}"
                         GIT_TAG "${__otb_module_tag}"
                         GIT_SUBMODULES "${__get_submodules}"
        )
    endif() # ${OTBGroup_${module_name}}
    math(EXPR __i "${__i}+1")
endwhile() # i LESS_EQUAL __nb_internal_modules

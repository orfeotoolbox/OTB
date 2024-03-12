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
# Index of lists must be coherent!
# When a new internal module is added, the four lists; otb_internal_modules, 
# otb_internal_modules_repos, otb_internal_modules_git_tag,
# otb_internal_modules_desc
list(APPEND otb_internal_modules
    "Miscellaneous")

list(APPEND otb_internal_modules_repos
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-modules/miscellaneous.git")

list(APPEND otb_internal_modules_git_tag
    "206b9c5919b4c4405ca71d371397e6475899c685")

list(APPEND otb_internal_modules_desc
"This module deals with image simulation algorithm. Using
objects transmittance and reflectance and sensor characteristics, it can be possible
to generate realistic hyperspectral synthetic set of data. This module includes
PROSPECT (leaf optical properties) and SAIL (canopy bidirectional reflectance)
models, as well as PROSAIL, which is the combination of the two previous ones."
)

set(i 0) # group index
list(LENGTH otb_internal_modules nb_internal_modules)

# for all enabled internal module, 
while(${i} LESS ${nb_internal_modules})
    list(GET otb_internal_modules "${i}" otb_module_name)
    # download only enabled modules
    if ("${OTBGroup_${otb_module_name}}")
        # Following variable is mandatory for otb_fetch compat
        set(Module_${otb_module_name} ON)
        list(GET otb_internal_modules_repos "${i}" otb_module_repo)
        list(GET otb_internal_modules_git_tag "${i}" otb_module_tag)
        list(GET otb_internal_modules_desc "${i}" otb_module_desc)

        message(STATUS "Downloading internal module ${module_name}")
        otb_fetch_module("${otb_module_name}"
            "${otb_module_desc}"
            "${CMAKE_CURRENT_SOURCE_DIR}/${otb_module_name}"
            GIT_REPOSITORY "${otb_internal_modules_repos}"
            GIT_TAG "${otb_internal_modules_git_tag}"
        )
    endif() # ${OTBGroup_${module_name}}
    math(EXPR i "${i}+1")
endwhile() # i LESS_EQUAL nb_internal_modules

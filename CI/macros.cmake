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
# This script is for the superbuild build on the CI platform

# Find the build name and CI profile
macro( set_dash_build_name )
  set(ci_profile wip)

  set(ci_mr_source "$ENV{CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}")
  set(ci_mr_target "$ENV{CI_MERGE_REQUEST_TARGET_BRANCH_NAME}")
  set(ci_mr_iid "$ENV{CI_MERGE_REQUEST_IID}")

  set(ci_pipeline_id "$ENV{CI_PIPELINE_ID}")
  set(ci_job_id "$ENV{CI_JOB_ID}")

  set(ci_ref_name "$ENV{CI_COMMIT_REF_NAME}")

  set (CTEST_BUILD_NAME ${ci_short_sha})

  if(ci_mr_source AND ci_mr_target AND ci_mr_iid)
    set (CTEST_BUILD_NAME "${ci_mr_source} (MR ${ci_mr_iid}) P-${ci_pipeline_id}")
    set(ci_profile mr)

  elseif(ci_ref_name)
    set (CTEST_BUILD_NAME "${ci_ref_name} P-${ci_pipeline_id}")

    if("${ci_ref_name}" STREQUAL "develop")
      set(ci_profile develop)

    elseif("${ci_ref_name}" MATCHES "^release-[0-9]+\\.[0-9]+\$")
      set(ci_profile release)
    endif()

  endif()

endmacro()

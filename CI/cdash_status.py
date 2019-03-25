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

import cdash_handler
import sys

if __name__ == "__main__":
  if ( len(sys.argv) < 5 ):
    print("Usage : "+sys.argv[0]+" commit_sha1 project_id site_name build_directory")
  handler = cdash_handler.Handler()
  handler.site = sys.argv[3]
  handler.build_dir = sys.argv[4]
  handler.GetName()
  handler.GetStamp()
  handler.GetBuildId()
  cdash_url = handler.GetBuildUrl()
  print ( cdash_url )
  gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
  gitlab_url += sys.argv[2] + "/statuses/" + sys.argv[1]
  gitlab_url += "?name=cdash:" + handler.name +"&state=success&target_url="
  gitlab_url += cdash_url
  print (handler.name)
  
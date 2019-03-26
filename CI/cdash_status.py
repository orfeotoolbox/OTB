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
import requests
import os.path
import sys

if __name__ == "__main__":
  if ( len(sys.argv) < 5 ):
    print("Usage : "+sys.argv[0]+" commit_sha1 project_id build_directory token")
  print (sys.argv)
  handler = cdash_handler.Handler()
  build_dir = os.path.join( sys.argv[3] , "")
  print("build_dir is ", build_dir)
  handler.build_dir = build_dir
  handler.GetSite()
  handler.GetName()
  handler.GetStamp()
  handler.GetBuildId()
  cdash_url = handler.GetBuildUrl()
  print ( cdash_url )
  gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
  gitlab_url += sys.argv[2] + "/statuses/" + sys.argv[1]
  params = {'name':'cdash:' + handler.name , 'state': 'success' ,\
   'target_url' : cdash_url}
  print (handler.name)
  headers = {'PRIVATE-TOKEN' : sys.argv[4] }
  gitlab_request=requests.post(gitlab_url, headers = headers, params = params)
  print (gitlab_request.url)
  print (gitlab_request.text)
  
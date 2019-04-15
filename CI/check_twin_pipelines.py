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

import os
import urllib.request
import urllib.parse
import json

"""
Check for any duplicated twin pipeline and cancel it
"""
if __name__ == "__main__":
  env = os.environ
  sha1 = env['CI_COMMIT_SHA']
  if 'CI_MERGE_REQUEST_IID' in env.keys():
    gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
    gitlab_url+= env['CI_PROJECT_ID'] + '/pipelines?sha='+sha1
    gitlab_request = urllib.request.Request(gitlab_url)
    gitlab_request.add_header('PRIVATE-TOKEN' , env['K8S_SECRET_TWIN_PIPELINE'] )
    res = urllib.request.urlopen(gitlab_request).read().decode()
    jres = json.loads(res)
    for item in jres:
      if item["id"] < int(env['CI_PIPELINE_ID']) and item["status"] == "running":
        gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
        gitlab_url+= env['CI_PROJECT_ID'] + '/pipelines/'+str(item["id"])+'/cancel'
        gitlab_request = urllib.request.Request(gitlab_url)
        gitlab_request.add_header('PRIVATE-TOKEN' , env['K8S_SECRET_TWIN_PIPELINE'] )
        res2 = urllib.request.urlopen(gitlab_request).read().decode()

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
import re
import time
import sys

"""
Send a request to Gitlab and return the answer
The request parameter is added after `project/:id/`
WARNING: when data is given, the request will be a POST
Otherwise, it is a GET
"""
def GitlabRequest(request, project=53, data=None, token=''):
  gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
  gitlab_url+= str(project) + '/' + request
  params = None
  myHeader = {}
  if not data is None:
    params = urllib.parse.urlencode(data).encode('ascii')
  if token:
    myHeader = {'PRIVATE-TOKEN':token}
  gitlab_request = urllib.request.Request(gitlab_url, data=params, headers=myHeader)
  res = urllib.request.urlopen(gitlab_request)
  return json.loads(res.read().decode())

"""
Check needed environment parameters
"""
def CheckEnvParameters(params):
  for p in params:
    if not p in os.environ.keys():
      print("Missing environment variable '"+p+"'")
      return False
  return True

"""
Check for any duplicated twin pipeline and cancel it
"""
if __name__ == "__main__":
  if not CheckEnvParameters(['CI_COMMIT_SHA']):
    sys.exit(1)
  env = os.environ
  sha1 = env['CI_COMMIT_SHA']
  # are we in a merge_request pipeline ?
  if 'CI_MERGE_REQUEST_IID' in env.keys():
    if not CheckEnvParameters(['K8S_SECRET_API_TOKEN']):
      print("Make sure you have set a valid acces token for Gitlab API")
      print("The K8S_SECRET_API_TOKEN environment variable should be set in 'Settings -> CI/CD -> Variables'")
      sys.exit(1)
    if not CheckEnvParameters(['CI_PROJECT_ID','CI_PIPELINE_ID']):
      sys.exit(1)
    mrInfo = GitlabRequest('merge_requests/'+env['CI_MERGE_REQUEST_IID'],token=env['K8S_SECRET_API_TOKEN'])
    wip_regex = re.compile("^[Ww][Ii][Pp]:")
    # is it a "WIP" merge request ?
    if wip_regex.search(mrInfo["title"]):
      # Yes: cancel the current pipeline
      print("Cancel current pipeline "+env['CI_PIPELINE_ID'])
      GitlabRequest('pipelines/'+env['CI_PIPELINE_ID']+'/cancel', data={}, \
        project=env['CI_PROJECT_ID'], token=env['K8S_SECRET_API_TOKEN'])
      time.sleep(180)
      print("Error: this pipeline should have been canceled")
      sys.exit(1)
    else:
      # No: cancel any previous "normal" pipeline on the same SHA1
      jres = GitlabRequest('pipelines?sha='+sha1, project=env['CI_PROJECT_ID'], token=env['K8S_SECRET_API_TOKEN'])
      for item in jres:
        if item["id"] < int(env['CI_PIPELINE_ID']) and item["status"] == "running":
          print("Cancel pipeline "+str(item["id"]))
          jres2 = GitlabRequest('pipelines/'+str(item["id"])+'/cancel', data={}, \
            project=env['CI_PROJECT_ID'], token=env['K8S_SECRET_API_TOKEN'])


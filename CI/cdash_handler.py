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
import os.path
import urllib.request
import urllib.parse
import glob
import re
import unittest
import sys
import json
import time
import xml.etree.ElementTree as ET


trace = True

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
Handler class to retrieve build informations
"""
class Handler:
  def __init__ (self):
    self.build_dir = ""
    self.configure_path = ""
    self.url = "https://cdash.orfeo-toolbox.org"
    self.project = "OTB"
    self.site = ""
    self.stamp = ""
    self.name = ""
    self.buildid = ""

  def GetConfigureFile (self , build_dir = "" ):
    if build_dir == "":
      build_dir = self.build_dir
    build_dir = os.path.normpath(build_dir)
    test_path = os.path.join( build_dir , "Testing/")
    list_folder = os.listdir(test_path)
    if trace:
      print(list_folder)
    configure_xml = ""
    for folder in list_folder:
      if folder == "Temporary" or folder == "TAG":
        pass
      configure_xml = os.path.join( test_path , folder , "Configure.xml" )
      if os.path.exists( configure_xml ):
        break
    if os.path.exists( configure_xml ):
      if trace:
        print ( configure_xml )
        #~ configure_file = open( configure_xml, "r" )
        #~ content = configure_file.read()
        #~ configure_file.close()
        #~ print( content )
      self.configure_path = configure_xml
      return self.configure_path
    print("Could not find the Configure.xml produced by ctest")
    sys.exit(1)

  def ParseConfigureFile(self):
    """
    Parse the configuration file to get Name, Site and BuildStamp
    """
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    # strip the Log section as it can mess up the XML parser
    startLog=content.find('<Log>')
    endLog=content.rfind('</Log>')
    if startLog > 0 and endLog > startLog:
      content = content[:(startLog+5)]+content[endLog:]
    # parse XML
    root = ET.fromstring(content)
    if trace:
      print( root.attrib )
    if not 'Name' in root.keys():
      print("Can't find site name in Configure.XML")
      sys.exit(1)
    if not 'BuildName' in root.keys():
      print("Can't find build name in Configure.XML")
      sys.exit(1)
    if not 'BuildStamp' in root.keys():
      print("Can't find build stamp in Configure.XML")
      sys.exit(1)
    self.site = root.get('Name')
    self.name = root.get('BuildName')
    self.stamp = root.get('BuildStamp')

  def GetBuildId (self, **kwargs):
    """
    This function is returning the buildid. Dict can be passed with the 
    different informations
    """
    site = self.site
    stamp = self.stamp
    name = self.name
    project = self.project
    for key , value in kwargs.items():
      if key == "site":
        site = value
      if key == "stamp":
        stamp = value
      if key == "name":
        name = value
      if key == "project":
        project = value
    if ( site == "" or stamp == "" or name == "" or project == ""):
      print( "Missing argument for buildid request site:"+site+", stamp:"+stamp+", name:"+name+", project:"+project+".")
      sys.exit(1)
    elif trace:
      print( "Argument for buildid request site:"+site+", stamp:"+stamp+", name:"+name+", project:"+project+".")
    buildid_api = "/api/v1/getbuildid.php?"
    buildid_params = urllib.parse.urlencode({'project': project, 'site': site, 'stamp': stamp , 'name': name})
    full_url = self.url + buildid_api + buildid_params
    if trace:
      print("full_url: "+full_url)
    nb_try = 6
    build_id_regex = re.compile( "<buildid>([0-9]+)</buildid>" )
    while nb_try:
      response = urllib.request.urlopen(full_url).read().decode()
      if trace:
        print ( "response: " + response )
      buildid = build_id_regex.search( response )
      nb_try -= 1
      if buildid or (nb_try == 0):
        break
      print("No build id, retry ...")
      time.sleep(60)
    if buildid:
      self.buildid = buildid.group(1)
      if trace:
        print ( "build id is ", self.buildid)
      return buildid.group(1)
    else:
      print("Error in recovering buildid")
      sys.exit(1)

  def GetBuildUrl (self , buildid = "" ):
    """
    This function is returning the build url. It can be called only when
    everything is set
    """
    if ( buildid == "" ):
      buildid = self.buildid
    if ( buildid == "" ):
      print( "Missing argument to build url")
      return
    build_url = self.url
    build_url +="/buildSummary.php?"
    build_url += "buildid=" + buildid
    return build_url

  def GetBuildStatus(self, buildid = "" ):
    """
    This function returns the status of a build id as a pair 'state' + 'errors'
    """
    if ( buildid == "" ):
      buildid = self.buildid
    if ( buildid == "" ):
      print( "Missing argument to build Status")
      return
    full_url = self.url + "/api/v1/buildSummary.php?buildid=" + buildid
    response = urllib.request.urlopen(full_url).read().decode()
    full_status = json.loads(response)
    state = "success"
    if full_status["configure"]["nerrors"] or full_status["build"]["nerrors"] \
    or full_status["test"]["nerrors"] or full_status["test"]["nfailed"]:
      state = 'failed'
    errors = ""
    if full_status["configure"]["nerrors"]:
      errors = "Errors occur during configure"
    elif full_status["build"]["nerrors"]:
      errors = "Errors occur during build"
    elif full_status["test"]["nerrors"] or full_status["test"]["nfailed"]:
      errors = "Errors occur during tests"
    return ( state , errors)

  def GetReturnValue(self, logfile):
    fd = open(logfile)
    content = fd.readlines()[0]
    fd.close()
    return int(content.strip("\n"))

  def GetLogStatus(self, logdir):
    """
    This function returns the log status of a build as a pair 'state' + 'errors'
    """
    configure_rv = os.path.join(logdir, "configure_return_value_log.txt")
    build_rv = os.path.join(logdir, "build_return_value_log.txt")
    test_rv = os.path.join(logdir, "test_return_value_log.txt")
    if os.path.exists( configure_rv ):
      if (self.GetReturnValue(configure_rv) != 0):
        return ( 'failed' , 'Configure failed')
    else:
      return ( 'failed' , 'Configure not run')
    if os.path.exists( build_rv ):
      if (self.GetReturnValue(build_rv) != 0):
        return ( 'failed' , 'Build failed')
    else:
      return ( 'failed' , 'Build not run')
    if os.path.exists( test_rv ):
      if (self.GetReturnValue(test_rv) != 0):
        return ( 'failed' , 'Tests failed')
    return ('success', '')

"""
  This script aims only at recovering the build url
  It uses environment variables setup by Gitlab Runner as default:
    CI_COMMIT_SHA             -> Commit SHA1
    CI_PROJECT_ID             -> Project ID
    CI_PROJECT_DIR            -> Project source directory
    K8S_SECRET_API_TOKEN      -> Token for Gitlab API
    CI_MERGE_REQUEST_REF_PATH -> Ref name to push the status (only for merge request pipeline)
    CI_COMMIT_REF_NAME        -> Ref name to push the status
  They can be overriden by a full command line :
    cdash_handler.py commit_sha1  project_id  project_directory  token  ref_name
"""
print("cdash_handler main")
if __name__ == "__main__":
  if trace:
    print(sys.argv)
  if ( len(sys.argv) < 6 and len(sys.argv) > 1 ):
    print("Usage : "+sys.argv[0]+" commit_sha1 project_id project_directory token ref_name")
    sys.exit(1)
  if ( len(sys.argv) >= 6):
    sha1 = sys.argv[1]
    proj = sys.argv[2]
    pdir = sys.argv[3]
    token = sys.argv[4]
    refn = sys.argv[5]
  else:
    if not CheckEnvParameters(['CI_COMMIT_SHA', 'CI_PROJECT_ID', 'CI_PROJECT_DIR', 'K8S_SECRET_API_TOKEN', 'CI_COMMIT_REF_NAME']):
      sys.exit(1)
    sha1 = os.environ['CI_COMMIT_SHA']
    proj = os.environ['CI_PROJECT_ID']
    pdir = os.environ['CI_PROJECT_DIR']
    token = os.environ['K8S_SECRET_API_TOKEN']
    if 'CI_MERGE_REQUEST_REF_PATH' in os.environ.keys():
      refn = os.environ['CI_MERGE_REQUEST_REF_PATH']
    else:
      refn = os.environ['CI_COMMIT_REF_NAME']
  handler = Handler()
  build_dir = os.path.join( pdir , "build/")
  if trace:
    print("build_dir is: " + build_dir)
  handler.build_dir = build_dir
  handler.GetConfigureFile()
  handler.ParseConfigureFile()
  if handler.GetBuildId() is None:
    cdash_url = "https://cdash.orfeo-toolbox.org"
    state = 'failed'
    error = "Failed to get build id"
  else:
    cdash_url = handler.GetBuildUrl()
    ( state , error ) = handler.GetLogStatus( os.path.join( pdir , "log") )
  if trace:
    print ( "cdash_url is: " + cdash_url )
  gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
  gitlab_url += proj + "/statuses/" + sha1
  params = urllib.parse.urlencode({'name':'cdash:' + handler.site , 'state': state ,\
   'target_url' : cdash_url , 'description' : error , 'ref' : refn })
  gitlab_request = urllib.request.Request(gitlab_url)
  gitlab_request.add_header('PRIVATE-TOKEN' , token )
  res = urllib.request.urlopen(gitlab_request, data=params.encode('ascii'))
  if trace:
    print ("gitlab_request.url: " + gitlab_request.full_url)
    print ("gitlab_request.text: " + res.read().decode())

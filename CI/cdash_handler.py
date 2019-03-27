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

import os.path
import requests
import glob
import re
import unittest


trace = False

class Handler:
# project
# site
# stamp
# name
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
        configure_file = open( configure_xml, "r" )
        content = configure_file.read()
        configure_file.close()
        print( content )
      self.configure_path = configure_xml
      return self.configure_path
    print("Could not find the Configure.xml produced by ctest")
    return

  def GetSite (self , build_dir="" ):
    """
    Site is corresponding to the Name field in the xml.
    """
    if ( build_dir == ""):
      build_dir = self.build_dir
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetSite function, could not find Configure.xml")
      return
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    site_regex = re.compile( "\\bName\\b=\"([0-9,\\s,\(,\),\-,\.,_,A-Z,a-z]+)")
    site = site_regex.search( content )
    if trace:
      print (site_regex)
      print(site)
    if site:
      if trace:
        print("site value \n" , site.group(1))
      self.site = site.group(1)
      return self.site
    print("Could not retreive site value")
    return
    return 

  def GetName (self , build_dir = ""):
    """
    This function is looking for the name information in the build tree: 
    which is BuildName
    """
    if ( build_dir == ""):
      build_dir = self.build_dir
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetName function, could not find Configure.xml")
      return
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    name_regex = re.compile( "\\bBuildName\\b=\"([0-9,\\s,\(,\),\-,\.,_,A-Z,a-z]+)\"")
    name = name_regex.search( content )
    if trace:
      print (name_regex)
      print( name)
    if name:
      if trace:
        print("name value \n" , name.group(1))
      self.name = name.group(1)
      return self.name
    print("Could not retreive name value")
    return

  def GetStamp (self , build_dir = "" ):
    """
    This function is looking for the stamp information in the build tree
    """
    if ( build_dir == ""):
      build_dir = self.build_dir
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetStamp function, could not find Configure.xml")
      return
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    stamp_regex = re.compile( "\\bBuildStamp\\b=\"([0-9,\\s,\(,\),\-,\.,_,A-Z,a-z]+)\"")
    stamp = stamp_regex.search( content )
    if trace:
      print( stamp_regex )
      print( stamp )
    if stamp:
      if trace:
        print("Stamp value \n" , stamp.group(1))
      self.stamp = stamp.group(1)
      return self.stamp
    print("Could not retreive stamp value")
    return

  def GetBuildId (self, **kwargs):
    """
    This function is returning the buildid. Dict can be passed with the 
    different informations
    """
    trace = True
    site = self.site
    stamp = self.stamp
    name = self.name
    project = self.project
    print( kwargs.items() )
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
      print( "Not enougth argument given for buildid request \
site:"+site+", stamp:"+stamp+", name:"+name+", project:"+project+".")
      # TODO
      return
    buildid_url = self.url + "/api/v1/getbuildid.php?"
    buildid_url += "project=" + project + "&"
    buildid_url += "site=" + site + "&"
    buildid_url += "stamp=" + stamp + "&"
    buildid_url += "name=" + name
    build_id_page = requests.get(buildid_url)
    if trace:
      print ( build_id_page.text )
    build_id_regex = re.compile( "<buildid>([0-9]+)</buildid>" )
    buildid = build_id_regex.search( build_id_page.text )
    if buildid:
      self.buildid = buildid.group(1)
      if trace:
        print ( "build id is ", self.buildid)
      return buildid.group(1)
    else:
      print("Error in recovering buildid")
      return False

  def GetBuildUrl (self , buildid ="" ):
    """
    This function is returning the build url. It can be called only when
    everything is set
    """
    if ( buildid == "" ):
      buildid = self.buildid
    if ( buildid == "" ):
      print( "Not enougth argument given to build url")
      return
    build_url = self.url
    build_url +="/buildSummary.php?"
    build_url += "buildid=" + buildid
    return build_url

"""
TODO :
 documentation, header, test if it is possible.
 the script aims only at recovering the build url

"""
if __name__ == "__main__":
  if ( len(sys.argv) < 5 ):
    print("Usage : "+sys.argv[0]+" commit_sha1 project_id build_directory token")
  if trace:
    print (sys.argv)
  handler = cdash_handler.Handler()
  build_dir = os.path.join( sys.argv[3] , "build/")
  if trace:
    print("build_dir is: " + build_dir)
  handler.build_dir = build_dir
  handler.GetSite()
  handler.GetName()
  handler.GetStamp()
  handler.GetBuildId()
  cdash_url = handler.GetBuildUrl()
  if trace:
    print ( "cdash_url is: " + cdash_url )
  gitlab_url = "https://gitlab.orfeo-toolbox.org/api/v4/projects/"
  gitlab_url += sys.argv[2] + "/statuses/" + sys.argv[1]
  params = {'name':'cdash:' + handler.site , 'state': 'success' ,\
   'target_url' : cdash_url}
  headers = {'PRIVATE-TOKEN' : sys.argv[4] }
  gitlab_request=requests.post(gitlab_url, headers = headers, params = params)
  if trace:
    print ("gitlab_request.url: " + gitlab_request.url)
    print ("gitlab_request.text: " + gitlab_request.text)

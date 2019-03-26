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

trace = True

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
      self.configure_path = configure_xml
      return True
    print("Could not find the Configure.xml produced by ctest")
    return False

  def GetSite (self , build_dir="" ):
    """
    Site is corresponding to the Name field in the xml.
    """
    if ( build_dir == ""):
      build_dir = self.build_dir
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetSite function, could not find Configure.xml")
      return False
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    site_regex = re.compile( "\\bName\\b=\"([0-9,\-,\.,_,A-Z,a-z]+)")
    site = site_regex.search( content )
    if trace:
      print (site_regex)
      print(site)
    if site:
      if trace:
        print("site value \n" , site.group(1))
      self.site = site.group(1)
      return True
    print("Could not retreive site value")
    return False
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
      return False
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    name_regex = re.compile( "\\bBuildName\\b=\"([0-9,\-,\.,_,A-Z,a-z]+)\"")
    name = name_regex.search( content )
    if trace:
      print (name_regex)
      print( name)
    if name:
      if trace:
        print("name value \n" , name.group(1))
      self.name = name.group(1)
      return True
    print("Could not retreive name value")
    return False

  def GetStamp (self , build_dir = "" ):
    """
    This function is looking for the stamp information in the build tree
    """
    if ( build_dir == ""):
      build_dir = self.build_dir
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetStamp function, could not find Configure.xml")
      return False
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    stamp_regex = re.compile( "\\bBuildStamp\\b=\"([0-9,\-,\.,_,A-Z,a-z]+)\"")
    stamp = stamp_regex.search( content )
    if trace:
      print( stamp_regex )
      print( stamp )
    if stamp:
      if trace:
        print("Stamp value \n" , stamp.group(1))
      self.stamp = stamp.group(1)
      return True
    print("Could not retreive stamp value")
    return False

  def GetBuildId (self, **kwargs):
    """
    This function is returning the buildid. Dict can be passed with the 
    different informations
    """
    site = self.site
    stamp = self.stamp
    name = self.name
    site = self.site
    for key , value in kwargs.items():
      if key == "site":
        site = value
      if key == "stamp":
        stamp == value
      if key == "name":
        name = value
      if key == "site":
        site = value
    if ( site == "" or stamp == "" or name == "" or site == ""):
      print( "Not enougth argument given for buildid request ")
      # TODO
      return
    buildid_url = self.url + "/api/v1/getbuildid.php?"
    buildid_url += "project=" + self.project + "&"
    buildid_url += "site=" + self.site + "&"
    buildid_url += "stamp=" + self.stamp + "&"
    buildid_url += "name=" + self.name
    build_id_page = requests.get(buildid_url)
    build_id_regex = re.compile( "<buildid>([0-9]+)</buildid>" )
    buildid = build_id_regex.search( build_id_page.text )
    if buildid:
      self.buildid = buildid.group(1)
      return True
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
      # TODO
      return
    build_url = self.url
    build_url +="/buildSummary.php?"
    build_url += "buildid=" + self.buildid
    return build_url

#TODO
# class TestHandler(unittest.TestCase):

#   def test_GetConfigureFile (self):
  
#   def test_GetName (self):

#   def test_GetStamp (self):

#   def test_GetBuildId (self):

#   def test_GetBuildUrl (self):

"""
TODO :
 documentation, header, test if it is possible.
 the script aims only at recovering the build url

"""
# The script needs the project name.
if __name__ == "__main__":
  handler = Handler()
  handler.build_dir = ""
  handler.GetStamp()
  print (handler.stamp)
  handler.GetSite()
  print (handler.site)
  handler.GetName()
  print (handler.name)
import os.path
import requests
import glob
import sys
import re
import unittest

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
    # print(list_folder)
    configure_xml = ""
    for folder in list_folder:
      if folder == "Temporary" or folder == "TAG":
        pass
      configure_xml = os.path.join( test_path , folder , "Configure.xml" )
      if os.path.exists( configure_xml ):
        break
    # print ( configure_xml )
    if os.path.exists( configure_xml ):
      self.configure_path = configure_xml
      return True
    print("Could not find the Configure.xml produced by ctest")
    return False

  def GetSite (self , build_dir ):
    """
    Site is corresponding to the Name field in the xml. As there are other 
    "Name" in it, it might be hard to find the right one.
    """
    return 

  def GetProject (self , build_dir ):
    """
    Could not find the project name in the configure file.
    """
    return

  def GetName (self , build_dir = ""):
    """
    This function is looking for the name information in the build tree
    """
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetName function, could not find Configure.xml")
      return False
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    name_regex = re.compile( "<Site.*BuildName=\"([0-9,-,_,A-Z,a-z]+)\".*<Configure>" , re.S )
    # print (name_regex)
    # print(content)
    name = name_regex.search( content )
    # print( name)
    if name:
      # print("name value \n" , name.group(1))
      self.name = name.group(1)
      return True
    print("Could not retreive name value")
    return False

  def GetStamp (self , build_dir = "" ):
    """
    This function is looking for the stamp information in the build tree
    """
    if self.configure_path == "" and not self.GetConfigureFile( build_dir ):
      print ("Error in GetStamp function, could not find Configure.xml")
      return False
    configure_file = open( self.configure_path, "r" )
    content = configure_file.read()
    configure_file.close()
    stamp_regex = re.compile( "<Site.*BuildStamp=\"([0-9,-]+[A-Z,a-z,0-9]+)\".*<Configure>" , re.S )
    # print (stamp_regex)
    # print(content)
    stamp = stamp_regex.search( content )
    # print( stamp)
    if stamp:
      # print("Stamp value \n" , stamp.group(1))
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
    buildid_url = handler.url + "/api/v1/getbuildid.php?"
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

  def GetBuildUrl (self):
    """
    This function is returning the build url. It can be called only when
    everything is set
    """
    build_url = self.url
    build_url +="/buildSummary.php?"
    build_url += "buildid=" + self.buildid
    return build_url

#TODO
class TestHandler(unittest.TestCase):

  def test_GetConfigureFile (self):
  
  def test_GetName (self):

  def test_GetStamp (self):

  def test_GetBuildId (self):

  def test_GetBuildUrl (self):



if __name__ == "__main__":
"""
TODO :
 documentation, header, test if it is possible.
 the script aims only at recovering the build url

"""
# The script needs the site name and the project name.

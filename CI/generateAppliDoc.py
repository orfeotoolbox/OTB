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

# -*- coding: utf-8 -*-

import sys
import string
import shutil
import os

def help():
  print("This script provides an easy way to generate the application \
    documentation in html format. It uses the class \
    otbWrapperApplicationHtmlDocGenerator class to do so. \
    Waits as input the OTB_Binary path and the output directory where the \
    doc will be generated.")

def main(argv):
  argc = len(argv)
  if (argc < 2):
    help()
    return

  otbbin = argv[1]
  outDir = argv[2] + "/" 
  docExe = otbbin + "/bin/otbApplicationEngineTestDriver otbWrapperApplicationHtmlDocGeneratorTest1 "
  cmakeFile = otbbin + "/CMakeCache.txt"  

  ## open CMakeCache.txt
  f = open(cmakeFile, 'r')
  # Extract the list of modules from CMakeCache.txt
  appliKey = "OTB_APPLICATIONS_NAME_LIST"
  appSorted = []
  for line in f:
    if line.find(appliKey) != -1 :
      # supress white space if any
      line2 = line.strip()
      # supress line return
      line = line.strip(" \n")
      appList = line.split("=")[1]
      appSortedTmp = appList.split(";")
      appSortedTmp.sort()
      for app in appSortedTmp :
        if app != "TestApplication" :
          appSorted.append(app)
      break
  #print "Found applications:"
  #print appSorted


  ## close CMakeCache.txt
  f.close() 
  
  # Extract the OTB_DIR_SOURCE path form CMakeCache.txt
  ## open CMakeCache.txt
  f = open(cmakeFile, 'r')
  for line in f:
    if line.find("OTB_SOURCE_DIR") != -1 :
      # supress white space if any
      otbDir = line.strip()
      # supress line return
      otbDir = line.strip(" \n")
      otbDir = otbDir.split("=")[1]
      break
  #print "OTB_SOURCE_DIR:" + otbDir
  
  ## close CMakeCache.txt
  f.close()
 

  ## Find the list of subdir Application to sort them
  appDir =  otbDir + "/Modules/Applications/"
  fileList = os.listdir(appDir)
  dirList = []
  for fname in fileList:
    if os.path.isdir(appDir+fname):
      if fname != "AppTest":
        dirList.append(fname)
  #print "Subdir in Application:"
  #print dirList 

  
  fout = open(outDir+"index.html", 'w')
  fout.write("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//ENhttp://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"></style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">")
  fout.write("<h1>The following applications are distributed with OTB.</h1>")
  fout.write("List of available applications:<br /><br />")

  count = 0
  for dirName in dirList:
    group = dirName
    if dirName.startswith("App") and len(dirName) > 4:
      group = dirName[3:]
    fout.write("<h2>"+group+"</h2>")
    fList = os.listdir(appDir+dirName+"/app")
    for app in appSorted :
       for fname in fList:
         # We assume that the class source file nane is otb#app#.cxx
         if fname.find("otb"+app+".cxx") != -1:
           print("Generating " + app + " ...")
           filename = outDir + app + ".html"
           filename_without_path = app + ".html"
           commandLine = docExe + " " + app + " " + otbbin + "/lib/otb/applications " + filename + " 1"
           os.system(commandLine)
    
           outLine = "<a href=\"" + filename_without_path + "\">" + app + "</a><br />"
           fout.write(outLine)
           count = count+1
           break

  if count != len(appSorted):
    print("Some application doc may haven't been generated:")
    print("Waited for " + str(len(appSorted)) + " doc, only " + str(count) + " generated...")
  else:
     print(str(count) + " application documentations have been generated...")

  fout.write("</body")
  fout.write("</html>") 
  fout.close()

if __name__ == "__main__":
    main(sys.argv)

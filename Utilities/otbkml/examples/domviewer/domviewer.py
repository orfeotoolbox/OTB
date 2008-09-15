#!/usr/bin/env python

# Copyright 2008, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# A simple graphical application that parses a local or remote KML or KMZ file
# and displays the parsed Features in a tree format.
# Usage: call ./domviewer.py from a terminal command line, then use the
# File > Open File command to load a file. You may also supply a file
# (local filesystem or over http) to open on the command line as
# ./domviewer.py input.kml

import os
import sys
import tempfile
import urllib2
import zipfile
from idlelib.TreeWidget import TreeItem, TreeNode
from Tkinter import *
from tkFileDialog import askopenfilename
from tkSimpleDialog import askstring
from tkMessageBox import showwarning
import kmldom

class DomTreeItem(TreeItem):
  def __init__(self, element):
    self.__element = element
    if element.has_name():
      self.__name = element.get_name()
    elif element.IsA(kmldom.Type_Document):
      self.__name = 'Unnamed Document'
    elif element.IsA(kmldom.Type_Folder):
      self.__name = 'Unnamed Folder'
    elif element.IsA(kmldom.Type_Placemark):
      self.__name = 'Unnamed Placemark'
    elif element.IsA(kmldom.Type_ScreenOverlay):
      self.__name = 'Unnamed ScreenOverlay'
    elif element.IsA(kmldom.Type_PhotoOverlay):
      self.__name = 'Unnamed PhotoOverlay'
    elif element.IsA(kmldom.Type_GroundOverlay):
      self.__name = 'Unnamed GroundOverlay'
    elif element.IsA(kmldom.Type_NetworkLink):
      self.__name = 'Unnamed NetworkLink'
    else:
      self.__name = 'Unknown Feature'

  def GetText(self):
    return self.__name

  def GetSubList(self):
    container = kmldom.AsContainer(self.__element)
    if container:
      size = container.get_feature_array_size()
      if size == 0: return
      child_list = []
      for i in range(container.get_feature_array_size()):
        child_list.append(container.get_feature_array_at(i))
      children = [DomTreeItem(child) for child in child_list]
      return children

  def IsExpandable(self):
    container = kmldom.AsContainer(self.__element)
    if container:
      return container.get_feature_array_size() > 0
    return False


class DomViewer:
  def __init__(self, root):
    self.__root = root
    self.__yscrollbar = Scrollbar(self.__root)
    self.__yscrollbar.pack(side=RIGHT, fill=Y)
    self.__canvas = Canvas(self.__root, yscrollcommand=self.__yscrollbar.set)
    self.__canvas.pack(fill=BOTH, expand=YES)
    self.__yscrollbar.config(command=self.__canvas.yview)
    self.__filetypes = [('KML or KMZ files', '*.km*')]
    self.__CreateMenus()

  def __CreateMenus(self):
    self.__app_menu = Menu(self.__root)
    self.__root.config(menu=self.__app_menu)
    self.__filemenu = Menu(self.__app_menu)
    self.__app_menu.add_cascade(label='File', menu=self.__filemenu)
    self.__filemenu.add_command(label='Open File...', command=self.__OpenFile)
    self.__filemenu.add_command(label='Open URL...', command=self.__OpenURL)
    self.__filemenu.add_command(label='Quit', command=self.__Quit)

  def __SetDom(self, dom):
    item = DomTreeItem(dom)
    node = TreeNode(self.__canvas, None, item)
    node.update()
    node.expand()

  def __GetRootFeature(self, element):
    kml = kmldom.AsKml(element)
    if kml:
      if kml.has_feature():
        return kml.get_feature()
      else:
        return None
    feature = kmldom.AsFeature(element)
    if feature:
      return feature
    return None

  def __ReadKmlFromKmz(self, filename):
    if not zipfile.is_zipfile(filename):
      showwarning('Error', '%s is not a valid KMZ file' % filename)
      return None
    zfile = zipfile.ZipFile(filename, 'r')
    if 'doc.kml' in zfile.namelist():
      kml = zfile.read('doc.kml')
    else:
      kml = zfile.read(zfile.namelist()[0])
    if not kml:
      showwarning('Error', 'Failed reading KML from KMZ')
      return None
    return kml

  def LoadFile(self, filename):
    # Public, can be called indirectly from command line.
    if zipfile.is_zipfile(filename):
      kml = self.__ReadKmlFromKmz(filename)
    else:
      kml = open(filename).read()  # consume the entire file
    dom = self.__GetRootFeature(kmldom.ParseKml(kml))
    if dom is None:
      showwarning('Bad KML', 'The KML data could not be parsed')
      return
    self.__SetDom(dom)

  def __OpenFile(self):
    f = askopenfilename(parent=self.__root, filetypes=self.__filetypes)
    if not f:
      return
    try:
      self.LoadFile(f)
    except IOError:
      showwarning('OpenFile', 'Cannot open the file')
      return

  def LoadUrl(self, url):
    # Public, can be called indirectly from command line.
    response = urllib2.urlopen(url).read()
    (fd, name) = tempfile.mkstemp()
    os.write(fd, response)
    os.close(fd)
    self.LoadFile(name)
    os.unlink(name)

  def __OpenURL(self):
    f = askstring('Open URL', 'URL', parent=self.__root,
                  initialvalue='http://')
    if not f:
      return
    try:
      self.LoadUrl(f)
    except:
      showwarning('OpenUrl', 'Cannot open the URL: %s' %f)
      return

  def __Quit(self, event=None):
    self.__root.quit()


def main(argv):
  if len(argv) > 2:
    print 'usage: %s kmlfile' % argv[0]
    sys.exit(1)
  root = Tk()
  app = DomViewer(root)
  if (len(argv) == 2):
    if argv[1].startswith('http://'):
      try: app.LoadUrl(argv[1])
      except: pass
    else:
      try: app.LoadFile(argv[1])
      except: pass
  root.mainloop()

if __name__ == '__main__':
  main(sys.argv)

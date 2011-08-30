# -*- coding: utf-8 -*-

#
#  Example on the use of the Smoothing 
#
from sys import argv
import otbApplication as otb
import thread

class PythonApplicationClass:
    "Python application class"

    def __init__(self, key):
        self.m_App = otb.Registry.CreateApplication(key)

    def SetParameterString( self, key, value ):
        self.m_App.SetParameterString(key, value)

    def SetParameterFloat( self, key, value ):
        self.m_App.SetParameterFloat(key, value)

    def ExecuteAndWriteOutput( self ):
        thread.start_new_thread(self.m_App.ExecuteAndWriteOutput())
        self.WatchThread();
        return

    def WatchThread( self ):
        print "je watch"
        return


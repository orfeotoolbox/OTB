#!/usr/bin/python
import fileinput,glob,string,sys,os
from os.path import join

path = join(sys.argv[1],"*")

files = glob.glob(path+"*.cxx")
for line in fileinput.input(files,inplace=1):
    lineno = 0
    lineno = string.find(line,"itkImage.h")
    if(lineno>0):
        line=line.replace("itkImage.h","otbImage.h")
    sys.stdout.write(line)
    

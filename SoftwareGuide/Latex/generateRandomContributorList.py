#!/usr/bin/python

import random

inputFilename = 'ContributorList.txt'
outputFilename = 'ContributorListRandom.tex'

inputFile=open(inputFilename, 'r')
contributors = inputFile.readlines()
inputFile.close()
#remove empty lines
contributors = filter(lambda x:len(x)>5, contributors)
#remove trailing return
contributors = map(lambda x: x[0:len(x)-1], contributors)
  
randomIndex = range(len(contributors))
random.shuffle(randomIndex)

outputFile=open(outputFilename, 'w')
for i in range(1,len(contributors)-1):
  outputFile.write(contributors[randomIndex[i]])
  outputFile.write(', ')
 
outputFile.write(contributors[randomIndex[len(contributors)-1]])

outputFile.close()

#!/usr/bin/python
import otbApplication
import os
import sys
import glob
from optparse import OptionParser

##############################################################################
# Parameters
linesep = os.linesep
pixeltypes = {' uchar' : 1, ' int8' : 0, ' uint8' : 1, ' int16' : 2, ' uint16': 3, ' int32' : 4, ' uint32' : 5, ' float' : 6, ' double': 7}

import re

#Special/Exceptional cases
def RstifyDescription(s):
    s = s.replace(':\n', ':\n\n')
    s = s.replace('\n', ' ')
    s = s.replace('*','\*')
    if not len(s) == 0 and not s.endswith('.'):
        s += '.'
    return s

def ConvertString(s):
    '''Convert a string for compatibility in txt dump'''
    s = s.strip()
    s = s.replace('*','\*')
    return s


def EncloseString(s):
    if not s.startswith("\"") :
        s = "\"" + s
    if not s.endswith("\""):
        s = s + "\""
    return s

def ExpandPath(filename,path,exp):
    if not exp:
        return filename
    else:
        # Avoid chasing our tails
        (head,tail) = os.path.split(filename)
        if len(tail) > 0:
            filename = tail
        for dir,dirs,files in os.walk(path):
            for file in files:
                if file == filename:
                    return os.path.join(dir,file)
        return os.path.join(path,filename)

def GetPixelType(value):
    # look for type
    foundcode = -1
    foundname = ""
    for ptypename, ptypecode in pixeltypes.iteritems():
        if value.endswith(ptypename):
            foundcode = ptypecode
            foundname = ptypename
            break
    return foundcode,foundname

def GetParametersDepth(paramlist):
    depth = 0
    for param in paramlist:
        depth = max(param.count("."),depth)
    return depth

def GenerateChoice(app,param,paramlist, count = 0):
    output = " Available choices are: " + linesep
    spaces = ' ' * count
    for (choicekey,choicename) in zip(app.GetChoiceKeys(param),app.GetChoiceNames(param)):
        output += linesep + spaces + "- **"+ ConvertString(choicename) + "**"
        choicedesc = app.GetParameterDescription(param+"."+choicekey)
        if len(choicedesc) >= 2:
            output+= " : " + ConvertString(choicedesc)
        output += linesep + linesep
        # List option associated to one choice
        options = []
        for p in paramlist:
            if p.startswith(param+"."+choicekey+"."):
                options.append(p)
        if len(options) > 0:
            count += 1
            spaces = ' ' * count
            for option in options:
                output+= linesep + spaces + "- **"+ ConvertString(app.GetParameterName(option))+ "** : " + RstifyDescription(app.GetParameterDescription(option)) + linesep
            output+= linesep
    return output

def GenerateParameterType(app,param):
    if app.GetParameterType(param) == otbApplication.ParameterType_Empty:
        return "Boolean"
    if app.GetParameterType(param) == otbApplication.ParameterType_Int \
       or app.GetParameterType(param) == otbApplication.ParameterType_Radius \
       or app.GetParameterType(param) == otbApplication.ParameterType_RAM:
        return "Int"
    if app.GetParameterType(param) == otbApplication.ParameterType_Float:
        return "Float"
    if app.GetParameterType(param) == otbApplication.ParameterType_String:
        return "String"
    if app.GetParameterType(param) == otbApplication.ParameterType_StringList:
        return "String list"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputFilename :
        return "Input File name"
    if app.GetParameterType(param) == otbApplication.ParameterType_OutputFilename :
        return "Output File name"
    if app.GetParameterType(param) == otbApplication.ParameterType_Directory :
        return "Directory"
    if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
        return "Choices"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputImage \
            or app.GetParameterType(param) == otbApplication.ParameterType_ComplexInputImage:
        return "Input image"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputVectorData:
        return "Input vector data"
    if app.GetParameterType(param) == otbApplication.ParameterType_OutputImage \
            or app.GetParameterType(param) == otbApplication.ParameterType_ComplexOutputImage :
        return "Output image"
    if app.GetParameterType(param) == otbApplication.ParameterType_OutputVectorData:
        return "Output vector data"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputImageList:
        return "Input image list"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputVectorDataList:
        return "Input vector data list"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputFilenameList :
        return "Input File name list"
    if app.GetParameterType(param) == otbApplication.ParameterType_ListView:
        return "List"
    if app.GetParameterType(param) == otbApplication.ParameterType_Group:
        return "Group"
    if app.GetParameterType(param) == otbApplication.ParameterType_InputProcessXML:
        return "XML input parameters file"
    if app.GetParameterType(param) == otbApplication.ParameterType_OutputProcessXML:
        return "XML output parameters file"

def FindLengthOfLargestColumnText(app,paramlist):
    output= ""
    colLength = [2] * 3
    for param in paramlist:
        if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
            for (choicekey,choicename) in zip(app.GetChoiceKeys(param),app.GetChoiceNames(param)):
                lenp= len(param + " " + choicekey)
                if colLength[0] < lenp:
                    colLength[0] = lenp
                lenpdescr = len(choicename)
                if colLength[2] < lenpdescr:
                    colLength[2] = lenpdescr
        else:
            if colLength[0] < len(param):
                colLength[0] = len(param)
            lenpdescr = len(app.GetParameterName(param))
            if colLength[2] < lenpdescr:
                colLength[2] = lenpdescr
        lenptype = len(GenerateParameterType(app,param))
        if colLength[1] < lenptype:
            colLength[1] = lenptype
    return colLength

def RstTableHeaderLine(strlist, listlen, delimiter):
    line = "+"
    for i in xrange(len(strlist)):
        line += delimiter * listlen[i] + '+'
    line += linesep
    return line

def RstTableHeading(strlist, listlen):
    heading = RstTableHeaderLine(strlist, listlen, '-')
    for i in xrange(len(strlist)):
         spaces = ' ' * ((listlen[i] - len(strlist[i])) )
         heading += '|' + strlist[i] +  spaces
    heading += '|' + linesep
    heading += RstTableHeaderLine(strlist, listlen, '=')
    return heading

def MakeText(text, size):
    dsize = (size - len(text))
    output= '|' + text  + ' ' * (dsize)
    return output

def GenerateParametersTable(app,paramlist):
    colLength = FindLengthOfLargestColumnText(app, paramlist)
    output = linesep + ".. [#] Table: Parameters table for " + ConvertString(app.GetDocName()) + "." + linesep + linesep
    headerlist = ["Parameter Key", "Parameter Type", "Parameter Description"]
    for i in xrange(len(headerlist)):
        colLength[i] = len(headerlist[i]) if colLength[i] < len(headerlist[i]) else colLength[i]
    output += RstTableHeading(headerlist, colLength)
    for param in paramlist:
        output += MakeText(param, colLength[0])
        output += MakeText(GenerateParameterType(app,param), colLength[1])
        output += MakeText(GenerateParameterType(app,param), colLength[2])
        output += '|' + linesep
        output += RstTableHeaderLine(headerlist, colLength, '-')
        if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
            for (choicekey,choicename) in zip(app.GetChoiceKeys(param),app.GetChoiceNames(param)):
                output += MakeText(param + " " + choicekey, colLength[0])
                output += MakeText(" *Choice*" ,colLength[1])
                output += MakeText(choicename, colLength[2])
                output += '|' + linesep
                output += RstTableHeaderLine(headerlist, colLength, '-')
    return output

def unique(seq):
    # order preserving
    checked = []
    for e in seq:
        if e not in checked:
            checked.append(e)
    return checked

def ApplicationParametersToRst(app,paramlist,deep = False,current=""):
    output = ""
    # First run
    if len(current)==0:
        output += "This section describes in details the parameters available for this application. Table [#]_ presents a summary of these parameters and the parameters keys to be used in command-line and programming languages. Application key is *" + app.GetName() + "* ."  + linesep
        output += GenerateParametersTable(app,paramlist)
        firstlevelparams = []
        for param in paramlist:
            paramsplit = param.partition(".")
            firstlevelparams.append(paramsplit[0])
        firstlevelparams = unique(firstlevelparams)

        if deep:
            for param in firstlevelparams:
                output += linesep
                output += "**" + ConvertString(app.GetParameterName(param)) + "**" + linesep
                output += RstifyDescription(app.GetParameterDescription(param))
                if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
                    output += GenerateChoice(app,param,paramlist)
                    output += linesep
                else:
                    output += linesep
                    output += ApplicationParametersToRst(app,paramlist,deep,param)
        else:
            output+= linesep
            for param in firstlevelparams:
                output+= "- **"+ ConvertString(app.GetParameterName(param))+ ":** " + RstifyDescription(app.GetParameterDescription(param))
                if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
                    output += GenerateChoice(app,param,paramlist)
                output += linesep + linesep
            output+=  linesep
    else:
        currentlevelparams = []
        for param in paramlist:
            if param.startswith(current+".") and param.count(".") == current.count(".")+1:
                currentlevelparams.append(param)
        if len(currentlevelparams) > 0:
            output+= linesep
            for param in currentlevelparams:
                output+= "- **"+ ConvertString(app.GetParameterName(param))+ ":** " + RstifyDescription(app.GetParameterDescription(param)) + linesep
                output+= ApplicationParametersToRst(app,paramlist,deep,param) + linesep
                if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
                    output += GenerateChoice(app,param,paramlist, 1)
            output+= linesep

    return output

def ApplicationParametersToRstV2(app,paramlist,deep = False,current=""):
    output = ""
    # current level
    level = 0
    # First run
    if len(current)==0:
        output += "This section describes in details the parameters available for this application. Table [#]_ presents a summary of these parameters and the parameters keys to be used in command-line and programming languages. Application key is *" + app.GetName() + "* ."  + linesep
        output += GenerateParametersTable(app,paramlist)
    else:
        level = len(current.split('.'))
    indentLevel = level
    if deep == False:
        indentLevel += 1
    # compute prefix
    bulletStyle = "-*+"
    prefix = ""
    if indentLevel > 0:
        prefix = (' ' * (indentLevel-1)) + bulletStyle[(indentLevel-1)%3] + ' '
    # find parameter for current param
    currentlevelparams = []
    for param in paramlist:
        if param.startswith(current) and len(param.split('.')) == level+1:
            currentlevelparams.append(param)
    if len(currentlevelparams) > 0:
        output+= linesep
        for param in currentlevelparams:
            if app.GetParameterType(param) == otbApplication.ParameterType_Group and level == 0:
                output+= prefix+"**["+ ConvertString(app.GetParameterName(param))+ "]**"
            else:
                output+= prefix+"**"+ ConvertString(app.GetParameterName(param))+ "**"
            descr =  RstifyDescription(app.GetParameterDescription(param))
            if len(descr):
                output+= ": "+descr
            if app.GetParameterType(param) ==  otbApplication.ParameterType_Choice:
                output+= " Available choices are: "
                additionalKeys = []
                for choiceKey in app.GetChoiceKeys(param):
                    additionalKeys.append(param+'.'+choiceKey)
                nextParamList = paramlist + tuple(additionalKeys)
            else:
                nextParamList = paramlist
            output+= linesep
            ret = ApplicationParametersToRstV2(app,nextParamList,deep,param)
            if indentLevel == 0 and len(ret)==0:
                output+= linesep
            output+= ret
        output+= linesep
    return output

def GetApplicationExampleCommandLine(app,idx):

    output = "%s%s%s\t%s" % ("::", linesep , linesep, "otbcli_")
    output+= ConvertString(app.GetName())
    for i in range(0, app.GetExampleNumberOfParameters(idx)):
        output+=" -" + app.GetExampleParameterKey(idx,i)+ " " + app.GetExampleParameterValue(idx,i)
    output += linesep + linesep
    return output

def GetApplicationExamplePythonSnippet(app,idx,expand = False, inputpath="",outputpath=""):
    appname = app.GetName()
    printable = []
    output = linesep + "::" + linesep + linesep
    output+= "\t#!/usr/bin/python" + linesep

    output+= linesep
    output+= "\t# Import the otb applications package" + linesep
    output+= "\timport otbApplication" + linesep + linesep
    output+= "\t# The following line creates an instance of the " + ConvertString(app.GetName()) + " application " + linesep
    output+= "\t" + ConvertString(app.GetName()) + " = otbApplication.Registry.CreateApplication(\"" + ConvertString(app.GetName()) + "\")" + linesep + linesep
    output+= "\t# The following lines set all the application parameters:" + linesep
    for i in range(0, app.GetExampleNumberOfParameters(idx)):
        param = app.GetExampleParameterKey(idx,i)
        value = app.GetExampleParameterValue(idx,i)
        paramtype = app.GetParameterType(param)
        paramrole = app.GetParameterRole(param)
        if paramtype == otbApplication.ParameterType_ListView:
            break
        if paramtype == otbApplication.ParameterType_Group:
            break
        if paramtype ==  otbApplication.ParameterType_Choice:
            #app.SetParameterString(param,value)
            output+= "\t" + appname + ".SetParameterString(" + EncloseString(param) + "," + EncloseString(value) + ")" + linesep
        if paramtype == otbApplication.ParameterType_Empty:
            app.SetParameterString(param,"1")
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+",\"1\")" + linesep
        if paramtype == otbApplication.ParameterType_Int \
                or paramtype == otbApplication.ParameterType_Radius \
                or paramtype == otbApplication.ParameterType_RAM:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterInt("+EncloseString(param)+", "+value+")" + linesep
        if paramtype == otbApplication.ParameterType_Float:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterFloat("+EncloseString(param)+", "+value + ")" + linesep
        if paramtype == otbApplication.ParameterType_String:
            # app.SetParameterString(param,value)
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(value)+")" + linesep
        if paramtype == otbApplication.ParameterType_StringList:
            values = value.split(" ")
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+", "+str(values)+")" + linesep
        if paramtype == otbApplication.ParameterType_InputFilename \
            or paramtype == otbApplication.ParameterType_OutputFilename \
            or paramtype == otbApplication.ParameterType_Directory:
            if paramrole == 0:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand)) + ")" + linesep
                printable.append(["in","file",ExpandPath(value,inputpath,expand)])
            elif paramrole == 1:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,outputpath,expand))+")" + linesep
                printable.append(["out","file",ExpandPath(value,inputpath,expand)])
        if paramtype == otbApplication.ParameterType_InputImage :
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"+linesep
            printable.append(["in","img",ExpandPath(value,inputpath,expand)])
        if paramtype == otbApplication.ParameterType_ComplexInputImage:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")" + linesep
            printable.append(["in","cimg",ExpandPath(value,inputpath,expand)])
        if paramtype == otbApplication.ParameterType_InputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")" + linesep
            printable.append(["in","vdata",ExpandPath(value,inputpath,expand)])
        if paramtype == otbApplication.ParameterType_OutputImage :
            foundcode,foundname = GetPixelType(value)
            if foundcode != -1:
                # app.SetParameterString(param,EncloseString(ExpandPath(value[:-len(foundname),outputpath,expand))))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value[:-len(foundname)],outputpath,expand))+")" + linesep
           #app.SetParameterOutputImagePixelType(param,foundcode)
                if foundcode == 1:
                    printable.append(["out","ucimg",ExpandPath(value[:len(foundname)],inputpath,expand)])
                else:
                    printable.append(["out","img",ExpandPath(value[:len(foundname)],inputpath,expand)])
                output += "\t" + appname + ".SetParameterOutputImagePixelType("+EncloseString(param)+", "+str(foundcode)+")" + linesep
            else:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
                output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")" + linesep
                printable.append(["out","img",ExpandPath(value,outputpath,expand)])
        if paramtype == otbApplication.ParameterType_ComplexOutputImage :
            # TODO: handle complex type properly
            # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
            output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")" + linesep
            printable.append(["out","cimg",ExpandPath(value,outputpath,expand)])
        if paramtype == otbApplication.ParameterType_OutputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
            output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")" + linesep
            printable.append(["out","vdata",ExpandPath(value,outputpath,expand)])
        if paramtype == otbApplication.ParameterType_InputImageList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param) + ", " + str(values) + ")" + linesep
        if paramtype == otbApplication.ParameterType_InputVectorDataList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            #app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+ ", " + str(values) + ")" + linesep
        output+=linesep
    output += "\t# The following line execute the application" + linesep
    output+= "\t" + appname + ".ExecuteAndWriteOutput()"+ linesep
    return output,printable

def GetApplicationExamplePython(app,idx):
    output, printable = GetApplicationExamplePythonSnippet(app,idx)
    output+= linesep
    return output

def RstHeading(text, delimiter):
    heading = text + linesep
    heading += delimiter * len(text)  + linesep
    heading += linesep
    return heading

def ApplicationToRst(appname):
    output = ""
    app = otbApplication.Registry.CreateApplication(appname)
    # TODO: remove this when bug 440 is fixed
    app.Init()
    output += RstHeading(app.GetDocName(), '^')
    output += app.GetDescription() + linesep * 2
    output += RstHeading("Detailed description", '-')
    output += app.GetDocLongDescription() + linesep * 2
    limitations = app.GetDocLimitations()
    output += RstHeading("Parameters", '-')
    depth = GetParametersDepth(app.GetParametersKeys())
    deep = depth > 0
    output += ApplicationParametersToRstV2(app,app.GetParametersKeys(),deep) + linesep
    if app.GetNumberOfExamples() > 1:
        output += RstHeading("Examples", '-') + linesep
        #output += appdetailslevel + "{Examples}" + "\\label{appexamples:" + appname + "}" + linesep
        for i in range(0,app.GetNumberOfExamples()):
            output += ":Example "+  str(i+1) + ':' + linesep + linesep
#            output += RstHeading("Example "+  str(i+1) , '-')
            output += app.GetExampleComment(i)
            output+= "To run this example in command-line, use the following: " + linesep
            output += linesep + GetApplicationExampleCommandLine(app,i)
            output+= "To run this example from Python, use the following code snippet: " + linesep
            output += GetApplicationExamplePython(app,i)
    elif app.GetNumberOfExamples() == 1:
        output += RstHeading("Example", '-')
        if( len(app.GetExampleComment(0)) > 1):
            output += app.GetExampleComment(0)
        output+= "To run this example in command-line, use the following: " + linesep
        output += GetApplicationExampleCommandLine(app,0)
        output+= "To run this example from Python, use the following code snippet: " + linesep
        output += GetApplicationExamplePython(app,0)

    if len(limitations)>=2:
        output += RstHeading("Limitations", '~')
#        output += ":Limitations:" + linesep + linesep
        output += ConvertString(app.GetDocLimitations()) + linesep + linesep

    output += RstHeading("Authors", '~')
#    output += ":Authors:" + linesep + linesep
    output += "This application has been written by " + ConvertString(app.GetDocAuthors()) + "." + linesep + linesep
    seealso = app.GetDocSeeAlso()
    if len(seealso) >=2:
        output += RstHeading("See Also", '~')
#        output += ":See Also:" + linesep + linesep
        output += "These additional resources can be useful for further information: " + linesep
        # hlink="<http://www.readthedocs.org/" + ConvertString(app.GetDocSeeAlso()) + ".html>`_ "
        # output += linesep + "`" + ConvertString(app.GetDocSeeAlso()) + " " + hlink + linesep + linesep
        output += linesep + ConvertString(app.GetDocSeeAlso()) + linesep + linesep

    return output

def GetApplicationTags(appname):
     app = otbApplication.Registry.CreateApplication(appname)
     return app.GetDocTags()

import shutil

def RstPageHeading(text):
    output = RstHeading(text, "=") + linesep
    output += ".. toctree::" + linesep
    output += "\t:maxdepth: 2" + linesep
    output += linesep + linesep
    return output

def GenerateRstForApplications():
    out = ""
    blackList = ["TestApplication", "Example"]
    appIndexFile = open('Applications.rst', 'w')
    appNames = [app for app in otbApplication.Registry.GetAvailableApplications() if app not in blackList]
    if not appNames:
			print 'No OTB applications available. Please check ITK_AUTOLOAD_PATH env variable'
			sys.exit(1)
    sectionTags = ["Image Manipulation","Vector Data Manipulation", "Calibration","Geometry", "Image Filtering","Feature Extraction","Stereo","Learning","Segmentation"]
    appIndexFile.write(RstPageHeading("Applications"))

    for tag in sectionTags:
        directory= "Applications/" + tag
        if not os.path.exists(directory):
            os.makedirs(directory)
        tag_ = tag.replace(' ', '_')
        appIndexFile.write('\tApplications/' + tag_ + '.rst' + linesep)
        #chapterIndexFile = open('Applications/' + tag + '.rst', 'w')
        #chapterIndexFile.write(RstPageHeading(tag))
        #print linesep + RstHeading(tag, '=')
        appsRemoved = []
        for appName in appNames:
            apptags = GetApplicationTags(appName)

            if apptags.count(tag) > 0:
                print "Generating " + appName + ".rst"
                #chapterIndexFile.write("\t" + tag + '/' + appName + linesep)
                appFile = open('Applications/app_'  + appName + '.rst', 'w')
                out = ApplicationToRst(appName)
                appFile.write(out)
                appFile.close()
                appsRemoved.append(appName)
        for appName in appsRemoved:
            appNames.remove(appName)
        #chapterIndexFile.close()

    misctag = "Miscellaneous" #should this be Utilities
    if not os.path.exists("Applications/" + misctag):
        os.makedirs("Applications/" + misctag)

    appIndexFile.write('\tApplications/' + misctag + linesep)
    appIndexFile.close()
    #miscChapterIndexFile = open("Applications/" + misctag + '.rst', 'w')
    #miscChapterIndexFile.write(RstPageHeading(misctag))
    for appName in appNames:
        print "Generating " + appName + ".rst"
        appFile = open("Applications/app_" +  appName + ".rst", 'w')
        out = ApplicationToRst(appName)
        appFile.write(out)
        appFile.close()
        #miscChapterIndexFile.write('\t' + misctag + '/' + appName + linesep)
        out = ""
    return out


# Start parsing options
parser = OptionParser(usage="Export application(s) to tex or pdf file.")
parser.add_option("-a",dest="appname",help="Generate rst only for this application (eg: OrthoRectification)")
parser.add_option("-m",dest="module",help="Generate rst only for this module (eg: Image Manipulation)")
(options, args) = parser.parse_args()

if not options.appname is None:
    out = ApplicationToRst(options.appname)
    #print out
else:
    GenerateRstForApplications()

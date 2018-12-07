#!/usr/bin/env python3
#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
import sys
import argparse

import otbApplication

linesep = os.linesep
pixeltypes = {' uchar' : 1, ' int8' : 0, ' uint8' : 1, ' int16' : 2, ' uint16': 3, ' int32' : 4, ' uint32' : 5, ' float' : 6, ' double': 7}


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
    for ptypename, ptypecode in pixeltypes.items():
        if value.endswith(ptypename):
            foundcode = ptypecode
            foundname = ptypename
            break
    return foundcode,foundname

def render_choice(app, key):
    "Render a choice parameter to rst"

    # First render all the choice values
    template_parameter_choice_entry = open("templates/parameter_choice_entry.rst").read()

    choice_keys = app.GetChoiceKeys(key)
    choice_names = app.GetChoiceNames(key)

    choice_entries = ""
    for (choice_key, choice_name) in zip(choice_keys, choice_names):
        # For the description, replace newlines by |br| because we are in a bullet list item
        choice_description = app.GetParameterDescription(key + "." + choice_key).replace("\n", " |br| ")
        choice_entries += template_parameter_choice_entry.format(
            name=choice_name,
            #key=choice_key, # if we want to show the key in choice parameter values
            description=choice_description
        )

    # Then render the full choice parameter
    template_parameter_choice = open("templates/parameter_choice.rst").read()

    return template_parameter_choice.format(
        name=app.GetParameterName(key),
        key=key,
        value="[" + "|".join(choice_keys) + "]",
        flags=rst_parameter_flags(app, key),
        description=app.GetParameterDescription(key),
        choices=choice_entries,
    )

def GetApplicationExamplePythonSnippet(app,idx,expand = False, inputpath="",outputpath=""):
    appname = "app"

    output = ""

    output+= "\timport otbApplication" + linesep + linesep
    output+= "\t" + appname + " = otbApplication.Registry.CreateApplication(\"" + app.GetName() + "\")" + linesep + linesep
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
            output+= "\t" + appname + ".SetParameterString(" + EncloseString(param) + "," + EncloseString(value) + ")"
        if paramtype == otbApplication.ParameterType_Bool:
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+","+EncloseString(value)+")"
        if paramtype == otbApplication.ParameterType_Int \
                or paramtype == otbApplication.ParameterType_Radius \
                or paramtype == otbApplication.ParameterType_RAM:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterInt("+EncloseString(param)+", "+value+")"
        if paramtype == otbApplication.ParameterType_Float:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterFloat("+EncloseString(param)+", "+value + ")"
        if paramtype == otbApplication.ParameterType_String:
            # app.SetParameterString(param,value)
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(value)+")"
        if paramtype == otbApplication.ParameterType_StringList:
            values = value.split(" ")
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+", "+str(values)+")"
        if paramtype == otbApplication.ParameterType_InputFilename \
            or paramtype == otbApplication.ParameterType_OutputFilename \
            or paramtype == otbApplication.ParameterType_Directory:
            if paramrole == 0:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand)) + ")"
            elif paramrole == 1:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,outputpath,expand))+")"
        if paramtype == otbApplication.ParameterType_InputImage :
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"
        if paramtype == otbApplication.ParameterType_ComplexInputImage:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"
        if paramtype == otbApplication.ParameterType_InputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"
        if paramtype == otbApplication.ParameterType_OutputImage :
            foundcode,foundname = GetPixelType(value)
            if foundcode != -1:
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value[:-len(foundname)],outputpath,expand))+")"
                output += "\n"
                output += "\t" + appname + ".SetParameterOutputImagePixelType("+EncloseString(param)+", "+str(foundcode)+")"
            else:
                output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")"
        if paramtype == otbApplication.ParameterType_ComplexOutputImage :
            # TODO: handle complex type properly
            # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
            output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")"
        if paramtype == otbApplication.ParameterType_OutputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
            output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")"
        if paramtype == otbApplication.ParameterType_InputImageList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param) + ", " + str(values) + ")"
        if paramtype == otbApplication.ParameterType_InputVectorDataList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            #app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+ ", " + str(values) + ")"
        output+=linesep
    output += linesep
    output+= "\t" + appname + ".ExecuteAndWriteOutput()"+ linesep
    return output

def rst_section(text, delimiter, ref=None):
    "Make a rst section title"

    output = ""

    if ref is not None:
        output += ".. _" + ref + ":\n\n"

    output += text + "\n" + delimiter * len(text) + "\n\n"
    return output

def rst_parameter_value(app, key):
    type = app.GetParameterType(key)
    if type == otbApplication.ParameterType_Bool:
        return "bool"
    elif (type == otbApplication.ParameterType_Int
       or type == otbApplication.ParameterType_Radius
       or type == otbApplication.ParameterType_RAM):
        return "int"
    elif type == otbApplication.ParameterType_Float:
        return "float"
    elif type == otbApplication.ParameterType_String:
        return "string"
    elif type == otbApplication.ParameterType_StringList:
        return "string1 string2..."
    elif (type == otbApplication.ParameterType_InputFilename
            or type == otbApplication.ParameterType_OutputFilename):
        return "filename"
    elif (type == otbApplication.ParameterType_InputImage
       or type == otbApplication.ParameterType_ComplexInputImage
       or type == otbApplication.ParameterType_OutputImage
       or type == otbApplication.ParameterType_ComplexOutputImage):
        return "image"
    elif (type == otbApplication.ParameterType_InputVectorData
       or type == otbApplication.ParameterType_OutputVectorData):
        return "vectorfile"
    elif type == otbApplication.ParameterType_Directory:
        return "directory"
    elif type ==  otbApplication.ParameterType_Choice:
        return "choice"
    elif type == otbApplication.ParameterType_InputImageList:
        return "image1 image2..."
    elif type == otbApplication.ParameterType_InputVectorDataList:
        return "vectorfile1 vectorfile2..."
    elif type == otbApplication.ParameterType_InputFilenameList :
        return "filename1 filename2..."
    elif type == otbApplication.ParameterType_ListView:
        if app.GetListViewSingleSelectionMode(key):
            return "string"
        else:
            return "string1 string2..."
    elif type == otbApplication.ParameterType_Group:
        return "TODO"
    elif (type == otbApplication.ParameterType_InputProcessXML
        or type == otbApplication.ParameterType_OutputProcessXML):
        return "filename.xml"
    else:
        return "value"

def rst_parameter_flags(app, key):
    if app.IsMandatory(key) and not app.HasValue(key):
        return "*Mandatory* "
    elif app.HasValue(key) and app.GetParameterType(key) != otbApplication.ParameterType_Group:
        return "*Default value: {}* ".format(app.GetParameterValue(key))
    else:
        return ""

def detect_abuse(app):
    fake_groups = {}
    keys = app.GetParametersKeys()

    # For each choice parameter
    for key in keys:
        if app.GetParameterType(key) == otbApplication.ParameterType_Choice:

            # Consider all its possible values
            for choice_key in app.GetChoiceKeys(key):
                fullkey = key + "." + choice_key

                # See if that value is also used as a group anywhere in the application
                for k in keys:
                    if k.startswith(fullkey) and k != fullkey:

                        # In that case, mark the first element of that group
                        if fullkey not in fake_groups.values():
                            fake_groups[k] = fullkey

    return fake_groups

def rst_parameters(app):
    output = ""
    template_parameter = open("templates/parameter.rst").read()
    template_parameter_group = open("templates/parameter_group.rst").read()

    fake_markers = detect_abuse(app)

    previous_level = 1
    for key in app.GetParametersKeys():
        type = app.GetParameterType(key)

        # If reducing level not on a group parameter, render a horizontal line
        current_level = 1 + key.count(".")
        if current_level < previous_level and type != otbApplication.ParameterType_Group:
            output += "\n\n------------\n\n"
        previous_level = current_level

        # Choice parameter values can act as groups
        # Detect that case to add a section title
        if key in fake_markers:
            output += rst_section(app.GetParameterName(fake_markers[key]) + " options", "^")

        if type == otbApplication.ParameterType_Group:
            output += template_parameter_group.format(
                name=rst_section(app.GetParameterName(key), "^"),
                description=app.GetParameterDescription(key)
            )

        elif type == otbApplication.ParameterType_Choice:
            output += render_choice(app, key)

        else:
            output += template_parameter.format(
                name=app.GetParameterName(key),
                key=key,
                value=rst_parameter_value(app, key),
                description=app.GetParameterDescription(key),
                flags=rst_parameter_flags(app, key),
            )

    return output

def render_example_cli(app, index):
    "Render a command line example to rst (includes indentation)"

    output = ""

    # Render comment
    if len(app.GetExampleComment(index)) > 0:
        output += "    # " + app.GetExampleComment(index) + "\n"

    output += "    otbcli_" + app.GetName()
    for i in range(app.GetExampleNumberOfParameters(index)):
        output += " -" + app.GetExampleParameterKey(index, i) + " " + app.GetExampleParameterValue(index, i)
    output += "\n"
    return output

def render_all_examples_cli(app):
    "Render all command line examples to rst"

    if app.GetNumberOfExamples() == 0:
        return "    # No example found"
    if app.GetNumberOfExamples() == 1:
        return render_example_cli(app, 0)
    else:
        output = ""
        for i in range(app.GetNumberOfExamples()):
            if i > 0:
                output += "\n"
            output += render_example_cli(app, i)
        return output

def render_all_examples_python(app):
    "Render all python examples to rst"
    output = ""
    for i in range(app.GetNumberOfExamples()):
        output += GetApplicationExamplePythonSnippet(app, i)
    return output

def render_limitations(app):
    "Render app DocLimitations to rst"

    limitations = app.GetDocLimitations()
    if limitations is None or len(limitations) == 0 or limitations == "None":
        return ""
    else:
        return rst_section("Limitation", "-") + limitations

def render_see_also(app):
    "Render app See Also to rst"

    see_also = app.GetDocSeeAlso()
    if len(see_also) > 2:
        print(app.GetName(), len(see_also), see_also)
    if see_also is None or len(see_also) < 2:
        return ""
    else:
        # TODO make links?
        return rst_section("See also", "-") + see_also

def ApplicationToRst(appname):
    app = otbApplication.Registry.CreateApplication(appname)

    # TODO: remove this when bug 440 is fixed
    app.Init()

    parameters = rst_parameters(app)

    output = open("templates/application.rst").read().format(
        heading=rst_section(app.GetName(), '='),
        description=app.GetDescription(),
        longdescription=app.GetDocLongDescription(),
        parameters=parameters,
        examples_cli=render_all_examples_cli(app),
        examples_python=render_all_examples_python(app),
        limitations=render_limitations(app),
        see_also=render_see_also(app)
    )

    return output

def GetApplicationTags(appname):
     app = otbApplication.Registry.CreateApplication(appname)
     return app.GetDocTags()

def RstPageHeading(text, maxdepth, ref=None):
    output = rst_section(text, "=", ref=ref)
    output += ".. toctree::" + linesep
    output += "\t:maxdepth: " + maxdepth + linesep
    output += linesep + linesep
    return output

def GenerateRstForApplications(rst_dir):
    out = ""
    blackList = ["TestApplication", "Example", "ApplicationExample"]
    allApps = None
    try:
        allApps = otbApplication.Registry.GetAvailableApplications( )
        print(allApps)
    except:
        print('error in otbApplication.Registry.GetAvailableApplications()')
        sys.exit(1)

    if not allApps:
        print('No OTB applications available. Please check OTB_APPLICATION_PATH env variable')
        sys.exit(1)

    writtenTags = []
    appNames = [app for app in allApps if app not in blackList]

    print("All apps: %s" % (appNames,))

    appIndexFile = open(rst_dir + '/Applications.rst', 'w')
    appIndexFile.write(RstPageHeading("Applications", "2", ref="apprefdoc"))
    for appName in appNames:
        tags = GetApplicationTags(appName)

        if not tags:
            print("No tags for application: "  +  appName)
            sys.exit(1)

        tag = tags[0]

        tag_ = tag
        if tag.find(' '):
            tag_ = tag.replace(' ', '_')

        if not tag_:
            print('empty tag found for ' + appName)

        if not tag_ in writtenTags:
            appIndexFile.write('\tApplications/' + tag_ + '.rst' + linesep)
            writtenTags.append(tag_)

        tagFileName = rst_dir + '/Applications/'  + tag_ + '.rst'
        if os.path.isfile(tagFileName):
            tagFile = open(tagFileName, 'a')
            tagFile.write("\tapp_" + appName + linesep)
            tagFile.close()
        else:
            tagFile = open(tagFileName, 'w')
            tagFile.write( RstPageHeading(tag, "1") )
            tagFile.write("\tapp_" + appName + linesep)
            tagFile.close()

        print("Generating " + appName + ".rst" +  " on tag " + tag_)
        appFile = open(rst_dir + '/Applications/app_'  + appName + '.rst', 'w')
        out = ApplicationToRst(appName)
        appFile.write(out)
        appFile.close()

    return out

if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export application(s) to rst file")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    args = parser.parse_args()

    GenerateRstForApplications(args.rst_dir)

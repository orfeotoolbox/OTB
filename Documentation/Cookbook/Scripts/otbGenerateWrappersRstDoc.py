#!/usr/bin/env python3
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

import os
import sys
import argparse
import re

import otbApplication
from otbApplication import ParameterType_Bool, ParameterType_Int, ParameterType_Radius, ParameterType_RAM, ParameterType_Float, ParameterType_String, ParameterType_StringList, ParameterType_InputFilename, ParameterType_OutputFilename, ParameterType_InputImage, ParameterType_OutputImage, ParameterType_InputVectorData, ParameterType_OutputVectorData, ParameterType_Directory, ParameterType_Choice, ParameterType_InputImageList, ParameterType_InputVectorDataList, ParameterType_InputFilenameList, ParameterType_ListView, ParameterType_Group

from otb_warnings import application_documentation_warnings

from rst_utils import rst_section, RstPageHeading

linesep = os.linesep

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
    pixeltypes = {' uchar' : 1, ' int8' : 0, ' uint8' : 1, ' int16' : 2, ' uint16': 3, ' int32' : 4, ' uint32' : 5, ' float' : 6, ' double': 7}
    # look for type
    foundcode = -1
    foundname = ""
    for ptypename, ptypecode in pixeltypes.items():
        if value.endswith(ptypename):
            foundcode = ptypecode
            foundname = ptypename
            break
    return foundcode,foundname

def GetApplicationExamplePythonSnippet(app,idx,expand = False, inputpath="",outputpath=""):
    appname = "app"
    output = ""

    output += ".. code-block:: python\n\n"
    # Render example comment
    if len(app.GetExampleComment(idx)) > 0:
        output += "\t# {}\n".format(app.GetExampleComment(idx))

    output += "\timport otbApplication" + linesep + linesep
    output += "\t" + appname + " = otbApplication.Registry.CreateApplication(\"" + app.GetName() + "\")" + linesep + linesep
    for i in range(0, app.GetExampleNumberOfParameters(idx)):
        param = app.GetExampleParameterKey(idx,i)
        value = app.GetExampleParameterValue(idx,i)
        paramtype = app.GetParameterType(param)
        paramrole = app.GetParameterRole(param)
        if paramtype == ParameterType_ListView:
            if app.GetListViewSingleSelectionMode(param):
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(value)+")"
            else:
                output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+", "+EncloseString(value)+")"
        if paramtype == ParameterType_Group:
            pass
        if paramtype ==  ParameterType_Choice:
            #app.SetParameterString(param,value)
            output+= "\t" + appname + ".SetParameterString(" + EncloseString(param) + "," + EncloseString(value) + ")"
        if paramtype == ParameterType_Bool:
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+","+EncloseString(value)+")"
        if paramtype == ParameterType_Int \
                or paramtype == ParameterType_Radius \
                or paramtype == ParameterType_RAM:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterInt("+EncloseString(param)+", "+value+")"
        if paramtype == ParameterType_Float:
            # app.SetParameterString(param,value)
            output += "\t" + appname + ".SetParameterFloat("+EncloseString(param)+", "+value + ")"
        if paramtype == ParameterType_String:
            # app.SetParameterString(param,value)
            output+= "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(value)+")"
        if paramtype == ParameterType_StringList:
            values = value.split(" ")
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+", "+str(values)+")"
        if paramtype == ParameterType_InputFilename \
            or paramtype == ParameterType_OutputFilename \
            or paramtype == ParameterType_Directory:
            if paramrole == 0:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand)) + ")"
            elif paramrole == 1:
                # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,outputpath,expand))+")"
        if paramtype == ParameterType_InputImage :
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"
        if paramtype == ParameterType_InputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,inputpath,expand)))
            output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value,inputpath,expand))+")"
        if paramtype == ParameterType_OutputImage :
            foundcode,foundname = GetPixelType(value)
            if foundcode != -1:
                output += "\t" + appname + ".SetParameterString("+EncloseString(param)+", "+EncloseString(ExpandPath(value[:-len(foundname)],outputpath,expand))+")"
                output += "\n"
                output += "\t" + appname + ".SetParameterOutputImagePixelType("+EncloseString(param)+", "+str(foundcode)+")"
            else:
                output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")"
        if paramtype == ParameterType_OutputVectorData:
            # app.SetParameterString(param,EncloseString(ExpandPath(value,outputpath,expand)))
            output += "\t" + appname +".SetParameterString("+EncloseString(param)+", "+ EncloseString(ExpandPath(value,outputpath,expand)) + ")"
        if paramtype == ParameterType_InputImageList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            # app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param) + ", " + str(values) + ")"
        if paramtype == ParameterType_InputVectorDataList:
            values = value.split(" ")
            values = [ExpandPath(val,inputpath,expand) for val in values]
            #app.SetParameterStringList(param,values)
            output += "\t" + appname + ".SetParameterStringList("+EncloseString(param)+ ", " + str(values) + ")"
        output+=linesep
    output += linesep
    output+= "\t" + appname + ".ExecuteAndWriteOutput()" + linesep + linesep
    return output

def render_choice(app, key):
    "Render a choice parameter to rst"

    # First render all the choice values
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
    return template_parameter_choice.format(
        name=app.GetParameterName(key),
        key=key,
        value="[" + "|".join(choice_keys) + "]",
        flags=rst_parameter_flags(app, key),
        description=app.GetParameterDescription(key),
        choices=choice_entries,
    )

def rst_parameter_value(app, key):
    "Render a parameter value to rst"

    type = app.GetParameterType(key)

    # ListView is a special case depending on its mode
    if type == ParameterType_ListView:
        if app.GetListViewSingleSelectionMode(key):
            return "string"
        else:
            return "string1 string2..."

    # For all other types it's a simple mapping
    values = {}
    values.update({ParameterType_Bool: "bool"})
    values.update(dict.fromkeys([ParameterType_Int, ParameterType_Radius, ParameterType_RAM], "int"))
    values.update({ParameterType_Float: "float"})
    values.update({ParameterType_String: "string"})
    values.update({ParameterType_StringList: "string1 string2..."})
    values.update(dict.fromkeys([ParameterType_InputFilename, ParameterType_OutputFilename], "filename [dtype]"))
    values.update({ParameterType_InputImage: "image"})
    values.update({ParameterType_OutputImage: "image [dtype]"})
    values.update(dict.fromkeys([ParameterType_InputVectorData, ParameterType_OutputVectorData], "vectorfile"))
    values.update({ParameterType_Directory: "directory"})
    values.update({ParameterType_Choice: "choice"})
    values.update({ParameterType_InputImageList: "image1 image2..."})
    values.update({ParameterType_InputVectorDataList: "vectorfile1 vectorfile2..."})
    values.update({ParameterType_InputFilenameList: "filename1 filename2..."})

    if type in values:
        return values[type]
    else:
        raise ValueError("Cannot show parameter value for type ", type)

def rst_parameter_flags(app, key):
    """
    Display the mandatory and default value flags of a parameter
    The display logic tries to follow the logic in WrapperCommandLineLauncher::DisplayParameterHelp
    The default value is formatted using GetParameterAsString to use the same formatting as the cli interface
    """

    if app.IsMandatory(key) and not app.HasValue(key):
        return "*Mandatory* "
    elif app.HasValue(key) and app.GetParameterType(key) != ParameterType_Group and app.GetParameterAsString(key) != "":
        return "*Default value: {}* ".format(app.GetParameterAsString(key))
    else:
        return ""

def detect_abuse(app):
    "Detect choice parameter values which are also used as groups"

    fake_groups = {}
    keys = app.GetParametersKeys()
    choice_keys = [k for k in keys if app.GetParameterType(k) == ParameterType_Choice]

    # For each choice parameter
    for key in choice_keys:

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

def render_parameters(app):
    "Render application parameters to rst"

    output = ""

    fake_markers = detect_abuse(app)

    previous_level = 1
    for key in app.GetParametersKeys():
        type = app.GetParameterType(key)

        # If reducing level not on a group parameter, render a horizontal line
        current_level = 1 + key.count(".")
        if current_level < previous_level and type != ParameterType_Group:
            output += "\n\n------------\n\n"
        previous_level = current_level

        # Choice parameter values can act as groups
        # Detect that case to add a section title
        if key in fake_markers:
            output += rst_section(app.GetParameterName(fake_markers[key]) + " options", "^")

        if type == ParameterType_Group:
            output += template_parameter_group.format(
                name=rst_section(app.GetParameterName(key), "^"),
                description=app.GetParameterDescription(key)
            )

        elif type == ParameterType_Choice:
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
        return rst_section("Limitations", "-") + limitations

def render_see_also(app):
    "Render app See Also to rst"

    see_also = app.GetDocSeeAlso()
    if see_also is None or len(see_also) < 2:
        return ""
    else:
        return rst_section("See also", "-") + "| " + see_also.replace("\n", "\n| ") # use line blocks for see also

def multireplace(string, replacements):
    "multiple string replace (from https://stackoverflow.com/a/6117124/5815110)"
    substrs = sorted(replacements, key=len, reverse=True)
    regexp = re.compile('|'.join(map(re.escape, substrs)))
    return regexp.sub(lambda match: replacements[match.group(0)], string)

def make_links(text, allapps):
    "Replace name of applications by internal rst links"

    rep = {appname: ":ref:`{}`".format(appname) for appname in allapps}
    return multireplace(text, rep)

def render_deprecation_string(app):
    if app.IsDeprecated():
        return "This application is deprecated and will be removed in a future release."
    else:
        return ""

def render_application(appname, allapps):
    "Render app to rst"

    # Create the application without logger to avoid the deprecation warning log
    app = otbApplication.Registry.CreateApplicationWithoutLogger(appname)

    # TODO: remove this when bug 440 is fixed
    app.Init()

    application_documentation_warnings(app)

    output = template_application.format(
        label=appname,
        deprecation_string=render_deprecation_string(app),
        heading=rst_section(app.GetName(), '='),
        description=app.GetDescription(),
        longdescription=make_links(app.GetDocLongDescription(), allapps),
        parameters=render_parameters(app),
        examples_cli=render_all_examples_cli(app),
        examples_python=render_all_examples_python(app),
        limitations=render_limitations(app),
        see_also=make_links(render_see_also(app), allapps)
    )

    return output

def GetApplicationTags(appname):
    # Create the application without logger to avoid the deprecation warning log
    app = otbApplication.Registry.CreateApplicationWithoutLogger(appname)
    return app.GetDocTags()

def GenerateRstForApplications(rst_dir):
    "Generate .rst files for all applications"

    blackList = ["TestApplication", "Example", "ApplicationExample"]
    allApps = otbApplication.Registry.GetAvailableApplications()

    if not allApps:
        raise RuntimeError("No OTB applications available. Please check OTB_APPLICATION_PATH env variable.")

    writtenTags = []
    appNames = [app for app in allApps if app not in blackList]

    appIndexFile = open(rst_dir + '/Applications.rst', 'w')
    appIndexFile.write(RstPageHeading("All Applications", "2", ref="apprefdoc"))

    print("Generating rst for {} applications".format(len(appNames)))

    for appName in appNames:

        # Get application first tag
        tags = list(GetApplicationTags(appName))
        if "Deprecated" in tags:
            tags.remove("Deprecated")
        if not tags or len(tags) == 0:
            raise RuntimeError("No tags for application: " + appName)

        tag = tags[0]
        tag_ = tag.replace(" ", "_")

        # Add it to the index (i.e. https://www.orfeo-toolbox.org/CookBook/Applications.html)
        if not tag in writtenTags:
            appIndexFile.write('\tApplications/' + tag_ + '.rst\n')
            writtenTags.append(tag)

        # Create or update tag index file (e.g. https://www.orfeo-toolbox.org/CookBook/Applications/Feature_Extraction.html)
        tagFileName = rst_dir + '/Applications/'  + tag_ + '.rst'
        if os.path.isfile(tagFileName):
            with open(tagFileName, 'a') as tagFile:
                tagFile.write("\tapp_" + appName + "\n")
        else:
            with open(tagFileName, 'w') as tagFile:
                tagFile.write( RstPageHeading(tag, "1") )
                tagFile.write("\tapp_" + appName + "\n")

        # Write application rst
        with open(rst_dir + '/Applications/app_'  + appName + '.rst', 'w',encoding='utf-8') as appFile:
            appFile.write(render_application(appName, appNames))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export application(s) to rst file")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    args = parser.parse_args()

    # Load rst templates
    template_application = open("templates/application.rst").read()
    template_parameter = open("templates/parameter.rst").read()
    template_parameter_group = open("templates/parameter_group.rst").read()
    template_parameter_choice_entry = open("templates/parameter_choice_entry.rst").read()
    template_parameter_choice = open("templates/parameter_choice.rst").read()

    GenerateRstForApplications(args.rst_dir)

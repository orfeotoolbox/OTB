#!/usr/bin/env python3
#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

import sys
import re

def parameter_warnings(app_warn, app, key):

    def warn(message):
        app_warn("Parameter '{}' ".format(key) + message)

    name = app.GetParameterName(key)
    description = app.GetParameterDescription(key)

    if name[-1] == " ":
        warn("name ends with a space")

    if ":" in name:
        warn("name contains a special character (:)")

    if "." in name:
        warn("name contains a special character (.)")

def application_documentation_warnings(app):
    "Emit warnings about application documentation"

    def warn(message):
        print("OTB Documentation Warning ({}): {}".format(app.GetName(), message), file=sys.stderr)

    description = app.GetDescription()
    longdescription = app.GetDocLongDescription()

    if re.search("\\n [a-zA-Z]", longdescription):
        warn("Application Long Description contains '\\n ' pattern (usually not intended)")

    if app.GetNumberOfExamples() == 0:
        warn("Application has no examples")

    keys = app.GetParametersKeys()
    for key in app.GetParametersKeys():
        parameter_warnings(warn, app, key)

    if "inxml" in keys and not keys[-2] == "inxml":
        warn("'inxml' parameter is not second from last parameter")

    if "outxml" in keys and not keys[-1] == "outxml":
        warn("'outxml' is not last parameter")


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

# Regex used to find examples usage
examples_usage_regex = r"\/\* Example usage:\n(\.\/\w+ (.*?))\*\/"

def rst_section(text, delimiter, ref=None):
    "Make a rst section title"

    output = ""

    if ref is not None:
        output += ".. _" + ref + ":\n\n"

    output += text + "\n" + delimiter * len(text) + "\n\n"
    return output

def RstPageHeading(text, maxdepth, ref=None):
    output = rst_section(text, "=", ref=ref)
    output += ".. toctree::\n"
    output += "\t:maxdepth: {}\n\n\n".format(maxdepth)
    return output


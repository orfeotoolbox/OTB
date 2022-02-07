#!/usr/bin/env python3
#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

import argparse

import otbApplication


def format_key_list(keys_str):
    """
    Format the key list
    :param keys_str: a string containing the keys separated by spaces
    :return: a string containing the keys ordered and separated by a coma and a space
    """
    key_list = keys_str.split(" ")
    key_list.sort()
    return ", ".join(key_list)

def GenerateRstForPythonAPi(rst_dir):
    " Generate the .rst file for the PythonAPI page"

    print("Generating rst for Python API")

    # Instenciate an ImageMetadata object to retrieve the keys
    imd = otbApplication.ImageMetadata()

    # Render the page
    output_python_api = template_python_api.format(
        key_list_double=format_key_list(imd.GetKeyListNum()),
        key_list_string=format_key_list(imd.GetKeyListStr()),
        key_list_l1d=format_key_list(imd.GetKeyListL1D()),
#        key_list_l2d=format_key_list(imd.GetKeyListL2D()),
        key_list_time=format_key_list(imd.GetKeyListTime())
    )
    
    # Write the page
    with open(rst_dir + '/PythonAPI.rst', 'w',encoding='utf-8') as new_rst_file:
        new_rst_file.write(output_python_api)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export the PythonAPI doc page to rst file")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    args = parser.parse_args()

    # Load rst template
    template_python_api = open("templates/PythonAPI.rst").read()

    GenerateRstForPythonAPi(args.rst_dir)

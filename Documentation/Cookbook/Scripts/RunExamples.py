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

import argparse
import glob
import subprocess
import os
from os.path import join
import re

def run_example(otb_root, otb_data, name):
    # Find binary
    binary_names = glob.glob(join("bin", name))
    if len(binary_names) == 0:
        raise RuntimeError("Can't find binary for {}".format(name))
    if len(binary_names) > 1:
        raise RuntimeError("Found {} binaries for {}".format(len(binary_names), name))
    binary = os.path.abspath(binary_names[0])

    # Find source file
    sources_files = glob.glob(join(otb_root, "Examples/*/" + name + ".cxx"))
    if len(sources_files) == 0:
        raise RuntimeError("Can't find source file for {}".format(name))
    if len(sources_files) > 1:
        raise RuntimeError("Found {} source files for {}".format(len(sources_files), name))
    filename = os.path.abspath(sources_files[0])

    # Extract example usage command arguments
    usage_rx = "\/\* Example usage:\n\.\/[a-zA-Z]+ (.*)\*\/"
    match = re.search(usage_rx, open(filename).read(), flags = re.MULTILINE | re.DOTALL)

    if not match:
        raise RuntimeError("Can't find example usage in {}".format(filename))

    example_args = match.group(1).replace("\\\n", "").split()

    # Execute example with otb_data as working directory, because paths are given relative to otb_data
    print("$ " + binary + " " + " ".join(example_args))
    subprocess.check_call([binary, *example_args], cwd=otb_data)

def main():
    parser = argparse.ArgumentParser(usage="Run one or all OTB cxx examples")
    parser.add_argument("otb_root", help="Path to otb repository")
    parser.add_argument("otb_data", help="Path to otb-data repository")
    parser.add_argument("--name", type=str, help="Example name (none for all examples)")
    args = parser.parse_args()

    if args.name:
        run_example(args.otb_root, args.otb_data, args.name)
    else:
        list_of_examples =[os.path.splitext(os.path.basename(f))[0] for f in glob.glob(join(args.otb_root, "Examples/*/*.cxx"))]
        for name in list_of_examples:
            try:
                run_example(args.otb_root, args.otb_data, name)
            except RuntimeError as e:
                print("Warning: ", e)

if __name__ == "__main__":
    main()

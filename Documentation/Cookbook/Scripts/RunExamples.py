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

from rst_utils import examples_usage_regex

blacklist = [
        "ApplicationExample", # does not produce a binary (uses the application engine)
        "LAIAndPROSAILToSensorResponse" # does not run, wrong arguments
    ]

def run_example(otb_root, name, dry_run):
    """
    Run an example by name
    Assumes the current working directory is an OTB build
    """

    if name in blacklist:
        return

    # Find binary in bin/
    binary_names = glob.glob(join("bin", name))
    if len(binary_names) == 0:
        raise RuntimeError("Can't find binary for {}".format(name))
    if len(binary_names) > 1:
        raise RuntimeError("Found {} binaries for {}".format(len(binary_names), name))
    binary = os.path.abspath(binary_names[0])

    # Find source file in otb_root/Examples/<tag>/name
    sources_files = glob.glob(join(otb_root, "Examples/*/" + name + ".cxx"))
    if len(sources_files) == 0:
        raise RuntimeError("Can't find source file for {}".format(name))
    if len(sources_files) > 1:
        raise RuntimeError("Found {} source files for {}".format(len(sources_files), name))
    filename = os.path.abspath(sources_files[0])

    # Extract example usage command arguments
    matches = list(re.finditer(examples_usage_regex, open(filename).read(), flags = re.MULTILINE | re.DOTALL))

    if len(matches) == 0:
        print("Warning: no usage for example: {}".format(sources_files[0]))
        return

    for match in matches:
        # Get command line call and print it
        example_args = match.group(2).replace("\\\n", "").split()
        print("$ " + binary + " " + " ".join(example_args))

        if not dry_run:
            otb_data = join(otb_root, "Data")
            # Make sure Output dir exists
            os.makedirs(join(otb_data, "Output"), exist_ok=True)

            # Execute the example with otb_data as working directory,
            # because paths are given relative to otb_data in the example usage
            subprocess.check_call([binary, *example_args], cwd=otb_data)

def main():
    parser = argparse.ArgumentParser(usage="Run one or all OTB cxx examples")
    parser.add_argument("otb_root", help="Path to otb repository")
    parser.add_argument("--name", type=str, help="Run only one example with then given name")
    parser.add_argument("-n", "--dry-run", action='store_true', help="Dry run, only print commands")
    parser.add_argument("-k", "--keep-going", action='store_true', help="Keep going after failing examples")
    args = parser.parse_args()

    if args.name:
        run_example(args.otb_root, args.name, dry_run=args.dry_run)
    else:
        list_of_examples =[os.path.splitext(os.path.basename(f))[0] for f in glob.glob(join(args.otb_root, "Examples/*/*.cxx"))]
        for name in list_of_examples:
            try:
                run_example(args.otb_root, name, dry_run=args.dry_run)
            except Exception as e:
                if args.keep_going:
                    print("Warning:", e)
                else:
                    raise

if __name__ == "__main__":
    main()

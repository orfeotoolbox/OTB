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
import os
import os.path
from os.path import join
from collections import defaultdict

from rst_utils import rst_section, RstPageHeading

list_of_examples = [
    "Examples/BasicFilters/BandMathFilterExample.cxx",
    "Examples/FeatureExtraction/LineSegmentDetectorExample.cxx",
]

def generate_examples_index(rst_dir):

    # Compute dictionary of tag -> (list of examples)
    tag_files = defaultdict(list)
    for filename in list_of_examples:
        tag = filename.split("/")[1]
        name, _ = os.path.splitext(filename.split("/")[2])

        tag_files[tag].append(join(tag, name + ".rst"))

    # Render index file and tag index files
    os.mkdir(join(rst_dir, "Examples"))
    index_f = open(join(rst_dir, "Examples.rst"), "w")
    index_f.write(RstPageHeading("Examples", 3, ref="cpp-examples"))

    for tag, examples_filenames in tag_files.items():
        tag_filename = join("Examples", tag + ".rst")
        index_f.write("\t" + tag_filename + "\n")

        with open(join(rst_dir, tag_filename), "w") as tag_f:
            tag_f.write(RstPageHeading(tag, 3))

            for examples_filename in examples_filenames:
                tag_f.write("\t" + examples_filename + "\n")

def indent(str):
    return "\n".join(["    " + line for line in str.split("\n")])

def example_extract_description(code):
    return code, ""

def render_example(filename, otb_root):
    "Render a cxx example to rst"

    # Read the source code of the cxx example
    code = open(join(otb_root, filename)).read()

    # Don't show the license header to make it nicer,
    # and the cookbook is already under a CC license
    examples_license_header = open("templates/examples_license_header.txt").read()
    code = code.replace(examples_license_header, "")

    # Extract the rst description
    code, rst_description = example_extract_description(code)

    # Render the template
    template_example = open("templates/example.rst").read()
    output_rst = template_example.format(
        label="example-" + root,
        heading=rst_section(name, "="),
        description=rst_description,
        code=indent(code),
    )

    return output_rst

if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export examples to rst")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    parser.add_argument("otb_root", help="OTB repository root")
    args = parser.parse_args()

    print("Generating rst for {} examples".format(len(list_of_examples)))

    # Generate example index and tag indexes
    generate_examples_index(join(args.rst_dir, "C++"))

    # Generate examples rst
    for filename in list_of_examples:
        name = os.path.basename(filename)
        tag = filename.split("/")[1]
        root, ext = os.path.splitext(name)

        os.mkdir(join(args.rst_dir, "C++", "Examples", tag))
        with open(join(args.rst_dir, "C++", "Examples", tag, root + ".rst"), "w") as output_file:
            output_file.write(render_example(filename, args.otb_root))

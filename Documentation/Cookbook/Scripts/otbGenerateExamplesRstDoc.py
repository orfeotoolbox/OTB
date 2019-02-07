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

    # Compute dictionary of (tag filename) -> (list of examples)
    tag_files = defaultdict(list)
    for filename in list_of_examples:
        tag = filename.split("/")[1]
        name, _ = os.path.splitext(filename.split("/")[2])

        tag_files[join("Examples", tag + ".rst")].append(join(tag, name + ".rst"))

    # Render index file and tag index files
    os.mkdir(join(rst_dir, "Examples"))
    index_f = open(join(rst_dir, "Examples.rst"), "w")
    index_f.write(RstPageHeading("Examples", 3, ref="cpp-examples"))

    for tag_filename, examples_filenames in tag_files.items():
        index_f.write("\t" + tag_filename + "\n")

        with open(join(rst_dir, tag_filename), "w") as tag_f:
            tag_f.write(RstPageHeading("tag index file", 3))

            for examples_filename in examples_filenames:
                tag_f.write("\t" + examples_filename + "\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export examples to rst")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    args = parser.parse_args()

    print("Generating rst for {} examples".format(len(list_of_examples)))

    generate_examples_index(join(args.rst_dir, "cpp"))

    template_example = open("templates/example.rst").read()

    for filename in list_of_examples:

        name = os.path.basename(filename)
        tag = filename.split("/")[1]
        root, ext = os.path.splitext(name)

        output_rst = template_example.format(
            label="example-" + root,
            heading=rst_section(name, "="),
            description="description goes here.",
            code="    auto x = 5;"
        )

        os.mkdir(join(args.rst_dir, "cpp", "Examples", tag))
        with open(join(args.rst_dir, "cpp", "Examples", tag, root + ".rst"), "w") as output_file:
            output_file.write(output_rst)

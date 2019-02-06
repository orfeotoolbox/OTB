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
import os.path
from os.path import join

from rst_utils import rst_section

list_of_examples = [
    "Examples/FeatureExtraction/LineSegmentDetectorExample.cxx"
]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export examples to rst")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    args = parser.parse_args()

    template_example = open("templates/example.rst").read()

    for filename in list_of_examples:

        name = os.path.basename(filename)
        root, ext = os.path.splitext(name)

        output_rst = template_example.format(
            label="example-" + root,
            heading=rst_section(name, "="),
            description="description goes here.",
            code="    auto x = 5;"
        )

        with open(join(args.rst_dir, "cpp", "examples", root + ".rst"), "w") as output_file:
            output_file.write(output_rst)

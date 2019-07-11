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
import re
import glob

from rst_utils import rst_section, RstPageHeading, examples_usage_regex

def generate_examples_index(rst_dir, list_of_examples):

    # Compute dictionary of tag -> (list of examples)
    tag_files = defaultdict(list)
    for filename in list_of_examples:
        tag = filename.split("/")[1]
        name, _ = os.path.splitext(filename.split("/")[2])

        tag_files[tag].append(join(tag, name + ".rst"))

    # Render index file and tag index files
    os.makedirs(join(rst_dir, "Examples"), exist_ok=True)
    index_f = open(join(rst_dir, "Examples.rst"), "w")
    index_f.write(RstPageHeading("C++ Examples", 3, ref="cpp-examples"))

    for tag, examples_filenames in sorted(tag_files.items()):
        tag_filename = join("Examples", tag + ".rst")
        index_f.write("\t" + tag_filename + "\n")

        with open(join(rst_dir, tag_filename), "w") as tag_f:
            tag_f.write(RstPageHeading(tag, 3))

            for examples_filename in examples_filenames:
                tag_f.write("\t" + examples_filename + "\n")

def indent(str):
    return "\n".join(["    " + line for line in str.split("\n")])

def cpp_uncomment(code):
    # Strip '//  '
    return "\n".join([line[4:] for line in code.split("\n")])

def render_example(filename, otb_root):
    "Render a cxx example to rst"

    # Read the source code of the cxx example
    code = open(join(otb_root, filename)).read()

    # Don't show the license header to make it nicer,
    # and the cookbook is already under a CC license
    examples_license_header = open("templates/examples_license_header.txt").read()
    code = code.replace(examples_license_header, "")

    # Extract usages
    example_usage = ""
    usage_matches = list(re.finditer(examples_usage_regex, code, flags = re.MULTILINE | re.DOTALL))

    examples_usage_template = open("templates/example_usage.rst").read()
    for match in usage_matches:
        example_usage += examples_usage_template.format(indent(match.group(1).strip()))

    # Don't show usage in example source
    code = re.sub(examples_usage_regex, "", code, flags = re.MULTILINE | re.DOTALL)

    # Make the link to the source code
    link_name = os.path.basename(filename)
    link_href = "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/raw/develop/" + filename + "?inline=false"

    # Read the description from the example .rst file if it exists
    example_rst_file = join(otb_root, filename.replace(".cxx", ".rst"))
    if os.path.isfile(example_rst_file):
        rst_description = open(example_rst_file).read()
    else:
        rst_description = ""

    # Render the template
    name = os.path.basename(filename)
    template_example = open("templates/example.rst").read()
    output_rst = template_example.format(
        label=name,
        heading=rst_section(name, "="),
        description=rst_description,
        usage=example_usage,
        code=indent(code.strip()),
        link_name=link_name,
        link_href=link_href
    )

    return output_rst

def main():
    parser = argparse.ArgumentParser(usage="Export examples to rst")
    parser.add_argument("rst_dir", help="Directory where rst files are generated")
    parser.add_argument("otb_root", help="OTB repository root")
    args = parser.parse_args()

    # Get list of cxx examples as relative paths from otb_root
    list_of_examples = [os.path.relpath(p, start=args.otb_root) for p in sorted(glob.glob(join(args.otb_root, "Examples/*/*.cxx")))]
    print("Generating rst for {} examples".format(len(list_of_examples)))

    # Generate example index and tag indexes
    generate_examples_index(join(args.rst_dir, "C++"), list_of_examples)

    # Generate examples rst
    for filename in list_of_examples:
        name = os.path.basename(filename)
        tag = filename.split("/")[1]
        root, ext = os.path.splitext(name)

        os.makedirs(join(args.rst_dir, "C++", "Examples", tag), exist_ok=True)
        with open(join(args.rst_dir, "C++", "Examples", tag, root + ".rst"), "w") as output_file:
            output_file.write(render_example(filename, args.otb_root))

if __name__ == "__main__":
    main()

#!/bin/sh
#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
BINARY_FILES=$(find "$OTB_INSTALL_DIR/lib" "$OTB_INSTALL_DIR/bin" -type f -exec file {} \; | grep -i ': elf ' | cut -f1 -d':')
# run patchelf
for bin_file in $BINARY_FILES; do
    if [ $bin_file != "$OTB_INSTALL_DIR/bin/patchelf" ]; then
            #echo "adding rpath to $bin_file"
            $OTB_INSTALL_DIR/bin/patchelf "--set-rpath" "$OTB_INSTALL_DIR/lib" $bin_file
    fi
done
echo "***** OTB installation is complete *****"

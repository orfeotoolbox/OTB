#!/bin/bash
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
# This script aims to identify the unreferenced authors to invite the team to
# check if they have already signed the contributor license agreement (CLA).
# When this is done, the author must be added to the reference list.


GITLAB_PROJECT_URL=https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils
GIT_CONTRIBUTORS=/tmp/git-contributors.txt
KNOWN_CONTRIBUTORS=/tmp/known-contributors.txt
UNKNOWN_CONTRIBUTORS=/tmp/unknown-contributors.txt

# According to the Git documentation, the reference ("HEAD" in the present
# case) must be passed on the command line when the standard input of "git
# shortlog" command is not a terminal. Without the reference in such
# situation, the output of "git shortlog" is empty.
git shortlog -es HEAD | cut -f 2- \
    | sort -u > ${GIT_CONTRIBUTORS}

curl -s ${GITLAB_PROJECT_URL}/raw/master/CI/contributors/known-contributors.txt \
    | sort -u > ${KNOWN_CONTRIBUTORS}

diff ${KNOWN_CONTRIBUTORS} ${GIT_CONTRIBUTORS} > ${UNKNOWN_CONTRIBUTORS}
if [ "$?" -ne "0" ] ; then
    echo "${UNKNOWN_CONTRIBUTORS}: "$(wc -l ${UNKNOWN_CONTRIBUTORS})
    cat ${UNKNOWN_CONTRIBUTORS}
    echo ""
    echo "WARNING: ***************************************************************"
    echo "WARNING: Unknown contributors found:"
    sed -n -e 's,^> ,WARNING: - ,p' ${UNKNOWN_CONTRIBUTORS}
    echo "WARNING: Check if they have signed the contributor license agreements."
    echo "WARNING: ***************************************************************"
    echo ""
    exit 1
else
    echo "All contributors are already known."
    exit 0
fi

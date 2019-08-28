#!/bin/bash

PATH=/usr/lib/ccache:/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin

GITLAB_PROJECT_URL=https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils
GIT_CONTRIBUTORS=/tmp/git-contributors.txt
KNOWN_CONTRIBUTORS=/tmp/known-contributors.txt
UNKNOWN_CONTRIBUTORS=/tmp/unknown-contributors.txt

git shortlog -es | cut -f 2- \
    | sort -u > ${GIT_CONTRIBUTORS}

curl -s ${GITLAB_PROJECT_URL}/raw/master/CI/contributors/known-contributors.txt \
    | sort -u > ${KNOWN_CONTRIBUTORS}

diff ${KNOWN_CONTRIBUTORS} ${GIT_CONTRIBUTORS} > ${UNKNOWN_CONTRIBUTORS}
if [ "$?" -ne "0" ] ; then
    echo ""
    echo "WARNING: ***************************************************************"
    echo "WARNING: Unknown contributors found:"
    sed -n -e 's,^> ,WARNING: - ,p' ${UNKNOWN_CONTRIBUTORS}
    echo "WARNING: Check if they have signed the contributor license agreements."
    echo "WARNING: ***************************************************************"
    echo ""
    exit 1
else
    echo "No unknown contributor found."
    exit 0
fi

#!/usr/bin/env bash
#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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



if [ $# -lt 2 ]; then
    echo "Usage: $0 <download_directory> <archive_output_directory>"
    exit 1
fi

export LC_ALL=C

DOWNLOAD_DIR=$(readlink -f "$1")
OUTPUT_DIR=$(readlink -f "$2")
WGET=$(which wget)


CUR_DIR="$( cd "$( dirname "$0" )" && pwd )"

SB_CMAKE_DIR_REL=$CUR_DIR/../../SuperBuild/CMake
SB_CMAKE_DIR=$(readlink -f "${SB_CMAKE_DIR_REL}")
cd "$CUR_DIR/../../" || echo "cannot cd to CUR_DIR/../../"
GIT_BRANCH=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')
#if a branch contains release-X.Y then xdk is taken from packages/xdk/OTB-X.Y
if [[ "$GIT_BRANCH" =~ release-* ]]; then
    VERSION=$(echo "${GIT_BRANCH}"| sed 's/.*release-//'| cut -d'-' -f 1)
else
    VERSION="develop"
fi
CMAKE_FILES=$(find "${SB_CMAKE_DIR}" -maxdepth 1 -type f -name "External_*")

DOWNLOAD_NAMES=

mkdir -p "${DOWNLOAD_DIR}"
cd "${DOWNLOAD_DIR}" || echo "cannot cd to DOWNLOAD_DIR"
echo "Downloading files to ${DOWNLOAD_DIR}/"
for cmake in ${CMAKE_FILES}; do
  download_links=$(grep -h -E '^[^#]*\"(ftp|http|https)://.*(\.tar\.gz|\.tar\.bz2|\.tgz|\.tar\.xz|\.zip|export=download).*\"' "${cmake}" |
		    grep -o -E '(ftp|http|https)://[^\"]*' | sort | uniq)
  for url in ${download_links}; do
    file_name=$(echo "${url}" | grep -o -E '[^\/]+$')
    if [ -z "$file_name" ]; then
      echo "invalid filename for url=${url}" && exit 1;
    fi
    download_name=$(grep -E -A 3 -B 3 "(ftp|http|https).+$file_name" "${cmake}" | grep -E -o 'DOWNLOAD_NAME .+' | cut -d ' ' -f 2-)
    $WGET -N "${url}"
    ret="$?"
    if [ $ret -gt 0 ] && [ $ret -ne 8 ]; then
       echo "Download failed for URL: '${url}'. wget finished with exit status '$ret'."
       exit 1;
    fi
    if [ -n "$download_name" ]; then
      mv "${file_name}" "${download_name}"
      file_name=$download_name
    fi
    DOWNLOAD_NAMES="${DOWNLOAD_NAMES} ${file_name}"
  done
done

ARCHIVE_NAME="SuperBuild-archives-$VERSION"
echo "Creating archive ${OUTPUT_DIR}/$ARCHIVE_NAME.tar.bz2"
cd "${OUTPUT_DIR}" || echo "cannot cd to OUTPUT_DIR"
touch "${DOWNLOAD_DIR}/OTBSuperBuild.readme"
tar -cjf "$ARCHIVE_NAME.tar.bz2" -C ${DOWNLOAD_DIR} ${DOWNLOAD_NAMES} OTBSuperBuild.readme
echo "Saving md5sum to ${OUTPUT_DIR}/$ARCHIVE_NAME.md5"
md5sum "$ARCHIVE_NAME.tar.bz2" > "$ARCHIVE_NAME.md5"

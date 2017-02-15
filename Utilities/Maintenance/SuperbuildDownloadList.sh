#!/bin/bash

export LC_ALL=C
if [ $# -eq 2 ]; then
DOWNLOAD_DIR=$(readlink -f $1)
OUTPUT_DIR=$(readlink -f $2)
else
echo 'Usage: '$0' <download_directory> <archive_output_directory>'
exit 1
fi

WGET="wget -nv -q"

SB_CMAKE_DIR_REL=$(dirname $0)/../../SuperBuild/CMake
SB_CMAKE_DIR=$(readlink -f ${SB_CMAKE_DIR_REL})

cd $(dirname $0)/../../

GIT_BRANCH=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')

#if a branch contains release-X.Y then xdk is taken from packages/xdk/OTB-X.Y
if [[ "$GIT_BRANCH" =~ release-* ]]; then
    VERSION=$(echo ${GIT_BRANCH}| sed 's/.*release-//'| cut -d'-' -f 1)
else
    VERSION="develop"
fi
DOWNLOAD_LIST=$(grep -h -E "^[^#]*\"https?://.*(\.tar\.gz|\.tar\.bz2|\.tgz|\.tar\.xz|\.zip|export=download).*\"" ${SB_CMAKE_DIR}/*.cmake | grep -o -E "https?://[^\"]*" | sed "s/\/\(download\)\?$//" | sort | uniq)

DOWNLOAD_NAMES=

mkdir -p ${DOWNLOAD_DIR}
cd ${DOWNLOAD_DIR}

echo "Downloading files to ${DOWNLOAD_DIR}/"

for url in ${DOWNLOAD_LIST}; do
  # clean log file

  file_name=$(echo "${url}" | grep -o -E "[^\/]+$")
  docs_google=$(echo "${file_name}" | grep -F "export=download")
  if [ "${docs_google}" = "" ]; then
      if [ -f "$file_name" ]; then
          echo "$file_name exists, skipping"
      fi;
      $WGET -N ${url}
      ret="$?"
   if [ $ret -gt 0 -a $ret -ne 8  ]; then
       echo "Download failed for URL: '${url}'. wget finished with exit status '$ret'."
       exit 1;
   fi
  else
    # look into the original file for a valid download name
    file_name=$(grep -h -A 3 -B 3 -F "${url}" ${SB_CMAKE_DIR}/*.cmake | grep -E "^[^#]*DOWNLOAD_NAME" | grep -o -E "[^ ]*\.(tar\.gz|tar\.bz2|tar\.xz|zip)" | uniq)
    if [ -z "$file_name" ]; then
        echo "Can't find correct filename for url ${url} : skip download"
        exit 1;
    else
        # echo "Download ${file_name}..."
        $WGET ${url} -O ${file_name}
    fi
  fi

  if [ "$file_name" != "" ]; then
      DOWNLOAD_NAMES="${DOWNLOAD_NAMES} ${file_name}"
  fi

done

ARCHIVE_NAME="SuperBuild-archives-$VERSION"
echo "Creating archive ${OUTPUT_DIR}/$ARCHIVE_NAME.tar.bz2"

cd ${OUTPUT_DIR}
tar -cjf $ARCHIVE_NAME.tar.bz2 -C ${DOWNLOAD_DIR} ${DOWNLOAD_NAMES}

echo "Saving md5sum to ${OUTPUT_DIR}/$ARCHIVE_NAME.md5"
md5sum $ARCHIVE_NAME.tar.bz2 > $ARCHIVE_NAME.md5

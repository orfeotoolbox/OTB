#/bin/bash

if [ $# -eq 2 ]; then
DOWNLOAD_DIR=$(readlink -f $1)
OUTPUT_DIR=$(readlink -f $2)
else
echo 'Usage: '$0' <download_directory> <archive_output_directory>'
exit 1
fi

SB_CMAKE_DIR_REL=$(dirname $0)/../../SuperBuild/CMake
SB_CMAKE_DIR=$(readlink -f ${SB_CMAKE_DIR_REL})

DOWNLOAD_LIST=$(grep -h -E "^[^#]*\"https?://.*(\.tar\.gz|\.tar\.bz2|\.tgz|\.tar\.xz|\.zip|export=download).*\"" ${SB_CMAKE_DIR}/*.cmake | grep -o -E "https?://[^\"]*" | sed "s/\/\(download\)\?$//" | sort | uniq)

DOWNLOAD_NAMES=

mkdir -p ${DOWNLOAD_DIR}
cd ${DOWNLOAD_DIR}

echo "Start downloads..."

for url in ${DOWNLOAD_LIST}; do
  # clean log file
  echo "" >download.log

  file_name=$(echo "${url}" | grep -o -E "[^\/]+$")
  docs_google=$(echo "${file_name}" | grep -F "export=download")
  if [ "${docs_google}" = "" ]; then
    echo "Download ${file_name}..."
    wget -N ${url} -o download.log
  else
    # look into the original file for a valid download name
    file_name=$(grep -h -A 3 -B 3 -F "${url}" ${SB_CMAKE_DIR}/*.cmake | grep -E "^[^#]*DOWNLOAD_NAME" | grep -o -E "[^ ]*\.(tar\.gz|tar\.bz2|tar\.xz|zip)" | uniq)
    if [ -z "$file_name" ]; then
      echo "Can't find correct filename for url ${url} : skip download"
    else
      echo "Download ${file_name}..."
      wget -O ${file_name} ${url} -o download.log
    fi
  fi

  download_errors=$(grep "ERROR" download.log)
  if [ "$download_errors" != "" ]; then
    echo "=== Error downloading ${file_name} ==="
    cat download.log
  else
    if [ "$file_name" != "" ]; then
      DOWNLOAD_NAMES="${DOWNLOAD_NAMES} ${file_name}"
    fi
  fi
done

rm download.log

echo "Create archive..."

cd ${OUTPUT_DIR}
tar -cjf SuperBuild-archives.tar.bz2 -C ${DOWNLOAD_DIR} ${DOWNLOAD_NAMES}

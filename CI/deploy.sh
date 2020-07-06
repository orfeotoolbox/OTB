#!/bin/bash
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

if [ "$CI_COMMIT_REF_NAME" = "develop" ] # check if the branch name is develop or not
then # we are on develop
  jobs_directory=/home/otbpush/test/$(date +%F)
else # we are on a release branch
  jobs_directory=/home/otbpush/test/staging
fi

echo "jobs_directory=${jobs_directory}"

# Create today's directory on serveur otb5-vm2
echo "Creating today's directory"
ssh otbpush@otb5-vm2.orfeo-toolbox.org mkdir -p ${jobs_directory}
# Delete latest
echo "Deleting latest directory"
if [ "$CI_COMMIT_REF_NAME" = "develop" ]
then # On develop
  ssh otbpush@otb5-vm2.orfeo-toolbox.org rm -rf /home/otbpush/test/latest
  # Create symilink
  ssh otbpush@otb5-vm2.orfeo-toolbox.org ln -s ${jobs_directory} /home/otbpush/test/latest
  # Cleaning old directory
  matching_dirs=$(ls -1 | grep -oE '^20[0-9]{2}-[0-9]{2}-[0-9]{2}$' | sort)
  history_length=11
  rm -rf "$(echo "$matching_dirs" | tr ' ' '\n' | head -n -${history_length})"
else # On release
  # Remove what is inside staging area
  ssh otbpush@otb5-vm2.orfeo-toolbox.org rm -rf ${jobs_directory}/*
fi

# Push package
echo "Pushing binary packages"
scp OTB-*.{run,zip} otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/.
# Push doc
echo "Pushing documentation"
scp {CookBook-*-html.tar.gz,OTB-Doxygen-*.tar.bz2} otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/.

# Create zip, tar.gz and tar.xy source
echo "Creating source tarball and zip"
git archive --format=zip -o OTB-sources-"$CI_COMMIT_SHORT_SHA".zip HEAD
git archive --format=tgz -o OTB-sources-"$CI_COMMIT_SHORT_SHA".tar.gz HEAD
git archive --format=tar.xz -o OTB-sources-"$CI_COMMIT_SHORT_SHA".tar.xz HEAD
# Remove old source file
echo "Removing old sources"
ssh otbpush@otb5-vm2.orfeo-toolbox.org \
rm ${jobs_directory}/OTB-sources-*.zip \
${jobs_directory}/OTB-sources-*.tar.*

# Push new source file
echo "Pushing new sources"
scp OTB-sources-"$CI_COMMIT_SHORT_SHA".* \
otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/

echo "${CI_COMMIT_SHA}" > ref.sha
echo "Pushing ref.sha"
scp ref.sha otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/

#Untar doc
if [ "$CI_COMMIT_REF_NAME" = "develop" ]
then
  #Cookbook
  ssh otbpush@otb5-vm2.orfeo-toolbox.org \
    tar -xf ${jobs_directory}/CookBook-*-html.tar.gz -C ${jobs_directory}/
  ssh otbpush@otb5-vm2.orfeo-toolbox.org \
    rm -rf /home/otbpush/test/CookBook/*
  ssh otbpush@otb5-vm2.orfeo-toolbox.org \
    mv ${jobs_directory}/CookBook-*/* /home/otbpush/test/CookBook/.

  # Doxygen
  ssh otbpush@otb5-vm2.orfeo-toolbox.org \
    rm -rf /home/otbpush/test/Doxygen/*
  # Strip first component of the tar (Doxygen/html/...)
  ssh otbpush@otb5-vm2.orfeo-toolbox.org \
    tar -xf ${jobs_directory}/OTB-Doxygen-*.tar.bz2 -C /home/otbpush/test/Doxygen/ --strip-components=1
fi

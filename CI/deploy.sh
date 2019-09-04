#!/bin/bash
# Configure git for tar.xz
git config tar.tar.xz.command "xz -c"

pack_suffix=""
if [ $1 = "develop" ] # check if the branch name is develop or not
then # we are on develop
  jobs_directory=/home/otbpush/test/$(date +%F)
else # we are on a release branch
  jobs_directory=/home/otbpush/test/staging
  if [ "$#" -eq 2 ]
    then # there is a rc tag, we need a suffix for packages
    pack_suffix=-$(echo "$2" | grep -o "rc[0-9]*") # this retrieve the rc number
  fi
fi

echo "jobs_directory=${jobs_directory}"
echo "pack_suffix=${pack_suffix}"

# Create today's directory on serveur otb5-vm2
echo "Creating today's directory"
ssh otbpush@otb5-vm2.orfeo-toolbox.org mkdir -p ${jobs_directory}
# Delete latest
echo "Deleting latest directory"
if [ $1 = "develop" ]
then # On develop
  ssh otbpush@otb5-vm2.orfeo-toolbox.org rm -rf /home/otbpush/test/latest
  # Create symilink
  ssh otbpush@otb5-vm2.orfeo-toolbox.org ln -s ${jobs_directory} /home/otbpush/test/latest
  # Cleaning old directory
  matching_dirs=$(ls -1 | grep -oE '^20[0-9]{2}-[0-9]{2}-[0-9]{2}$' | sort)
  history_length=11
  rm -rf $(echo $matching_dirs | tr ' ' '\n' | head -n -${history_length})
else # On release
  # Remove what is inside staging area
  ssh otbpush@otb5-vm2.orfeo-toolbox.org rm -rf ${jobs_directory}/*
fi

# Push package
echo "Renaming binary packages"
for name in $(find . -regex "./OTB-.*\(run\|zip\)"); do
  len=(${#name})
  mv "$name" "${name:0:$len-4}${pack_suffix}${name:$len-4}"
done

echo "Pushing binary packages"
scp OTB-*.{run,zip} otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/.
# Push doc
echo "Pushing documentation"
scp {CookBook-*-html.tar.gz,OTB-Doxygen-*.tar.bz2} otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/.

# Create zip, tar.gz and tar.xy source
echo "Creating source tarball and zip"
git archive --format=zip -o OTB-sources-$CI_COMMIT_SHORT_SHA.zip HEAD
git archive --format=tgz -o OTB-sources-$CI_COMMIT_SHORT_SHA.tar.gz HEAD
git archive --format=tar.xz -o OTB-sources-$CI_COMMIT_SHORT_SHA.tar.xz HEAD
# Remove old source file
echo "Removing old sources"
ssh otbpush@otb5-vm2.orfeo-toolbox.org \
rm ${jobs_directory}/OTB-sources-*.zip \
${jobs_directory}/OTB-sources-*.tar.*

# Push new source file
echo "Pushing new sources"
scp OTB-sources-$CI_COMMIT_SHORT_SHA.* \
otbpush@otb5-vm2.orfeo-toolbox.org:${jobs_directory}/

 

#!/bin/bash
# This script aims at moving and renaming source, packages and documentation from
# staging area to production area

staging_area=/home/otbpush/test/staging
prod_area=/home/otbpush/test/prod
#check if we are dealing with the right packages

pack_suffix=-$(echo "$CI_COMMIT_TAG" | grep -o "rc[0-9]*") # this retrieve the rc number
echo "pack_suffix=${pack_suffix}"

echo "Renaming binary packages"
# find build_packages/. -name "*.run" \
# -exec sh -c 'mv "$1" "${1%.run}${pack_suffix}.run"' _ {} \;
packages_list=$(ls ${staging_area} | grep -E "OTB-[0-9]*.[0-9]*.[0-9]*-.*\.(run|zip)")
ls -all build_packages/
for name in $packages_list
  do
  len=${#name}
  #add pack_suffix to the name
  mv "$name" "${name:0:$len-4}${pack_suffix}${name:$len-4}"
done

# TO REMOVE
###########
ls -all build_packages/
###########

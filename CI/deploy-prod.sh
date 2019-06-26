#!/bin/bash
# This script aims at moving and renaming source, packages and documentation from
# staging area to production area

staging_area=/home/otbpush/test/staging
prod_area=/home/otbpush/test/prod
#check if we are dealing with the right packages
server_name="otbpush@otb5-vm2.orfeo-toolbox.org"
pack_sha=`ssh ${server_name} head ${staging_area}/ref.sha`
if [[ "${pack_sha}" != "${CI_COMMIT_SHA}" ]]
  then
  echo "Wrong sha1"
  exit 1
fi

pack_suffix=-`echo "$CI_COMMIT_TAG" | grep -o "rc[0-9]*"` # this retrieve the rc number
echo "pack_suffix=${pack_suffix}"

echo "Renaming binary packages"
# find build_packages/. -name "*.run" \
# -exec sh -c 'mv "$1" "${1%.run}${pack_suffix}.run"' _ {} \;
file_list=`ssh ${server_name} ls ${staging_area}`
 # | grep -E "OTB-[0-9]*.[0-9]*.[0-9]*-.*\.(run|zip)"

# TO REMOVE
###########
ssh ${server_name} ls -all ${staging_area}
###########

for name in ${file_list}
  do
  is_package=`echo ${name} | grep -E "OTB-[0-9]*.[0-9]*.[0-9]*-.*\.(run|zip)"`
  if [ ${is_package} ] # it is a package, we need to change the name
    then
    len=${#name}
    ssh ${server_name} cp ${staging_area}/${name} \
${prod_area}/${name:0:$len-4}${pack_suffix}${name:$len-4}
  else
    ssh ${server_name} cp ${staging_area}/${name} ${prod_area}/${name}
  fi
done

# TO REMOVE
###########
ssh ${server_name} ls -all ${prod_area}
###########


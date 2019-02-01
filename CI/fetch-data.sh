#!/bin/bash

# Fetch corresponding OTB-Data

if ! git ls-remote --exit-code https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data.git $CI_COMMIT_REF_NAME
then
  echo "Branch $CI_COMMIT_REF_NAME not found, using master"
  CI_COMMIT_REF_NAME="master"
fi

curl -s -S -L -o "otb-data-$CI_COMMIT_REF_NAME.tar.gz" \
    "https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data/-/archive/$CI_COMMIT_REF_NAME/otb-data-$CI_COMMIT_REF_NAME.tar.gz"
tar xzf "otb-data-$CI_COMMIT_REF_NAME.tar.gz"
mv "otb-data-$CI_COMMIT_REF_NAME" otb-data
rm -f "otb-data-$CI_COMMIT_REF_NAME.tar.gz"

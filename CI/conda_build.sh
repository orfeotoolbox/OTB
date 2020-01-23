#!/usr/bin/env bash

source /build/anaconda3/bin/activate

cd /build/staged-recipes/recipes/

conda config --set anaconda_upload yes
anaconda login --username ${ANACONDA_LOGIN} --password ${ANACONDA_PASSWORD} --hostname ${CI_JOB_ID}

conda build ${CONDA_BUILD_OPTIONS} muparserx
conda build ${CONDA_BUILD_OPTIONS} openthreads
conda build ${CONDA_BUILD_OPTIONS} ossim
conda build ${CONDA_BUILD_OPTIONS} shark
conda build ${CONDA_BUILD_OPTIONS} tinyxml
conda build ${CONDA_BUILD_OPTIONS} libitk
conda build ${CONDA_BUILD_OPTIONS} otb

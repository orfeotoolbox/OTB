# This script is a prototype for the futur CI
SET( ENV{LANG} "C" ) # Only ascii output

SET (CTEST_SOURCE_DIRECTORY "/opt/otb/")
SET (CTEST_BINARY_DIRECTORY "/opt/otb/build/")

SET (DATA_ROOT_DIR "/opt/otb/otb-data/") # todo

ctest_start("Nightly")

# ctest_update() no need to update it is done by Gitlab-CI

ctest_configure()
ctest_build()
ctest_test()
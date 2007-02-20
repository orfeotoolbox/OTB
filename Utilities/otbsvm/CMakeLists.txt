PROJECT(OTBSVM)

# source files for zlib
SET(OTBSVM_SRCS
svm.cxx 
)

ADD_LIBRARY(otbsvm ${OTBSVM_SRCS})
INSTALL_TARGETS(/lib/otb otbsvm)
INSTALL_FILES(/include/otb/Utilities/otbsvm svm.h)

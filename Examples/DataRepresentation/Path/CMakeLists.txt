PROJECT(PathExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(PolyLineParametricPath1 PolyLineParametricPath1.cxx )
TARGET_LINK_LIBRARIES(PolyLineParametricPath1 OTBCommon OTBIO)

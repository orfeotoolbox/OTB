project(PathExamples)
include_regular_expression("^.*$")

add_executable(PolyLineParametricPath1 PolyLineParametricPath1.cxx )
target_link_libraries(PolyLineParametricPath1 OTBCommon OTBIO)

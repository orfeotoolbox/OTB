otb_module_test()

set(OTBMonteverdiTests
mvdMonteverdiApplicationTest.cxx
mvdMonteverdiTestDriver.cxx
)

add_executable(mvdMonteverdiTestDriver ${OTBMonteverdiTests})
target_link_libraries(mvdMonteverdiTestDriver ${OTBMonteverdi-Test_LIBRARIES})
otb_module_target_label(mvdMonteverdiTestDriver)

# Tests Declaration
# TODO

cmake_minimum_required(VERSION 2.8.3)

project(boost)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/boost
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.ipp")


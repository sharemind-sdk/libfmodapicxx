#
# This file is a part of the Sharemind framework.
# Copyright (C) Cybernetica AS
#
# All rights are reserved. Reproduction in whole or part is prohibited
# without the written consent of the copyright owner. The usage of this
# code is subject to the appropriate license agreement.
#

CMAKE_MINIMUM_REQUIRED(VERSION "2.8.12")
PROJECT(SHAREMIND_LIBFMODAPICXX "CXX")

SET(LIBFMODAPICXX_VERSION "0.1.0.0")
SET(LIBFMODAPICXX_COMPAT_VERSION "0.1.0.0")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibmodapiCxx 0.3.0 REQUIRED)
FIND_PACKAGE(SharemindLibfmodapi 0.1.0 REQUIRED)

SET(LIBFMODAPICXX_INSTALL_DEFINITIONS
    ${SharemindCxxHeaders_DEFINITIONS}
    ${SharemindLibmodapiCxx_DEFINITIONS}
    ${SharemindLibfmodapi_DEFINITIONS})
LIST(REMOVE_DUPLICATES LIBFMODAPICXX_INSTALL_DEFINITIONS)
SET(LIBFMODAPICXX_INSTALL_LINK_LIBRARIES
    ${SharemindCxxHeaders_LIBRARIES}
    ${SharemindLibmodapiCxx_LIBRARIES}
    ${SharemindLibfmodapi_LIBRARIES})
LIST(REMOVE_DUPLICATES LIBFMODAPICXX_INSTALL_LINK_LIBRARIES)
SET(LIBFMODAPICXX_INSTALL_INCLUDE_DIRS
    "${CMAKE_INSTALL_PREFIX}/include"
    ${SharemindCxxHeaders_INCLUDE_DIRS}
    ${SharemindLibmodapiCxx_INCLUDE_DIRS}
    ${SharemindLibfmodapi_INCLUDE_DIRS})
LIST(REMOVE_DUPLICATES LIBFMODAPICXX_INSTALL_INCLUDE_DIRS)

FILE(GLOB_RECURSE SHAREMIND_LIBFMODAPICXX_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_libfmodapicxx_headers_in_IDE"
                  SOURCES ${SHAREMIND_LIBFMODAPICXX_HEADERS})
INSTALL(FILES ${SHAREMIND_LIBFMODAPICXX_HEADERS}
        DESTINATION "include/sharemind/libfmodapi")

# Install cmake files:
FILE(GLOB_RECURSE sharemind_libfmodapicxx_in_files
    "${CMAKE_CURRENT_SOURCE_DIR}/*.in")
ADD_CUSTOM_TARGET("include_sharemind_libfmodapicxx_in_files_in_IDE"
                  SOURCES ${sharemind_libfmodapicxx_in_files})
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/SharemindLibfmodapiCxxConfig.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/SharemindLibfmodapiCxxConfig.cmake"
               @ONLY)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/SharemindLibfmodapiCxxConfigVersion.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SharemindLibfmodapiCxxConfigVersion.cmake" @ONLY)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/SharemindLibfmodapiCxxConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/SharemindLibfmodapiCxxConfigVersion.cmake"
        DESTINATION "lib/SharemindLibfmodapiCxx")

#
# Copyright (C) 2015 Cybernetica
#
# Research/Commercial License Usage
# Licensees holding a valid Research License or Commercial License
# for the Software may use this file according to the written
# agreement between you and Cybernetica.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl-3.0.html.
#
# For further information, please contact us at sharemind@cyber.ee.
#

CMAKE_MINIMUM_REQUIRED(VERSION "2.8.12")
PROJECT(SHAREMIND_LIBFMODAPICXX "CXX")

SET(LIBFMODAPICXX_VERSION "0.1.0.0")
SET(LIBFMODAPICXX_COMPAT_VERSION "0.1.0.0")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibModapiCxx 0.3.0 REQUIRED)
FIND_PACKAGE(SharemindLibFModapi 0.1.0 REQUIRED)

SET(LIBFMODAPICXX_INSTALL_DEFINITIONS
    ${SharemindCxxHeaders_DEFINITIONS}
    ${SharemindLibModapiCxx_DEFINITIONS}
    ${SharemindLibFModapi_DEFINITIONS})
LIST(REMOVE_DUPLICATES LIBFMODAPICXX_INSTALL_DEFINITIONS)
SET(LIBFMODAPICXX_INSTALL_LINK_LIBRARIES
    ${SharemindCxxHeaders_LIBRARIES}
    ${SharemindLibModapiCxx_LIBRARIES}
    ${SharemindLibFModapi_LIBRARIES})
LIST(REMOVE_DUPLICATES LIBFMODAPICXX_INSTALL_LINK_LIBRARIES)
SET(LIBFMODAPICXX_INSTALL_INCLUDE_DIRS
    "${CMAKE_INSTALL_PREFIX}/include"
    ${SharemindCxxHeaders_INCLUDE_DIRS}
    ${SharemindLibModapiCxx_INCLUDE_DIRS}
    ${SharemindLibFModapi_INCLUDE_DIRS})
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

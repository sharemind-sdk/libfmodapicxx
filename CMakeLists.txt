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

CMAKE_MINIMUM_REQUIRED(VERSION "3.0")
PROJECT(SharemindLibFModapiCxx VERSION "0.1.1" LANGUAGES "CXX")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers REQUIRED)

SharemindSetProjectVersion()


FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibModapiCxx 0.3.0 REQUIRED)
FIND_PACKAGE(SharemindLibFModapi 0.1.0 REQUIRED)


# Headers:
FILE(GLOB_RECURSE SharemindLibFModapiCxx_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_libfmodapicxx_headers_in_IDE"
                  SOURCES ${SharemindLibFModapiCxx_HEADERS})
INSTALL(FILES ${SharemindLibFModapiCxx_HEADERS}
        DESTINATION "include/sharemind/libfmodapi"
        COMPONENT "dev")

# Install cmake files:
SharemindCreateCMakeFindFiles(
    INCLUDE_DIRS
        "${CMAKE_INSTALL_PREFIX}/include"
        ${SharemindCxxHeaders_INCLUDE_DIRS}
        ${SharemindLibModapiCxx_INCLUDE_DIRS}
        ${SharemindLibFModapi_INCLUDE_DIRS}
    DEFINITIONS
        ${SharemindCxxHeaders_DEFINITIONS}
        ${SharemindLibModapiCxx_DEFINITIONS}
        ${SharemindLibFModapi_DEFINITIONS}
    LIBRARIES
        ${SharemindCxxHeaders_LIBRARIES}
        ${SharemindLibModapiCxx_LIBRARIES}
        ${SharemindLibFModapi_LIBRARIES}
)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("dev"
    NAME "libsharemind-fmodapi-cxx-dev"
    DESCRIPTION "Sharemind Facility Module API C++ wrapper development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-fmodapi-dev (>= 0.1.0)"
        "libsharemind-modapi-cxx-dev (>= 0.3.0)"
        "libsharemind-cxxheaders-dev"
        "libstdc++-dev"
)
SharemindPackagingFinalize()

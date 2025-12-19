# Install script for directory: /home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "default")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/lib/libns3.38-nr-u-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so"
         OLD_RPATH "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-nr-u-default.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/nr-on-off-access-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/nr-lbt-access-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/application-scenario.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/interference-application.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/in-front-node-distribution.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/l2-setup.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/no-interference-node-distribution.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/node-distribution-scenario.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/nr-single-bwp-setup.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/single-pair-node-scenario.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/varying-interference-node-distribution.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/wigig-setup.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/output-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/file-output-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/half-matrix-layout.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/model/wifi-setup.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/nr-u/helper/nr-u-trace-helper.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/include/ns3/nr-u-module.h"
    )
endif()


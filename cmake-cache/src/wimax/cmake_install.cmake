# Install script for directory: /home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/lib/libns3.38-wimax-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so"
         OLD_RPATH "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.38-wimax-default.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/helper/wimax-helper.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-channel.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-net-device.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-net-device.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ss-net-device.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/cid.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/cid-factory.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ofdm-downlink-frame-prefix.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-connection.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ss-record.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/mac-messages.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/dl-mac-messages.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ul-mac-messages.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-phy.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/simple-ofdm-wimax-phy.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/simple-ofdm-wimax-channel.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/send-params.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/service-flow.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ss-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/connection-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-mac-header.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-mac-queue.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/crc8.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/service-flow-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-uplink-scheduler.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-uplink-scheduler-simple.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-uplink-scheduler-mbqos.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-uplink-scheduler-rtps.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ul-job.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-scheduler.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-scheduler-simple.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-scheduler-rtps.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/service-flow-record.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/snr-to-block-error-rate-record.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/snr-to-block-error-rate-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/simple-ofdm-send-param.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ss-service-flow-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bs-service-flow-manager.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/cs-parameters.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ipcs-classifier-record.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-tlv.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/ipcs-classifier.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/bvec.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/src/wimax/model/wimax-mac-to-mac-header.h"
    "/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/build/include/ns3/wimax-module.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/shohruh/ns-3-ML-dev/ML-Framework-for-NR-U-MAC-Protocol-Design-Multi-agent/cmake-cache/src/wimax/examples/cmake_install.cmake")

endif()


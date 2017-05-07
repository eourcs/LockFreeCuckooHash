# Install script for directory: /Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/city_hasher.hh;/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/cuckoohash_config.hh;/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/cuckoohash_map.hh;/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/cuckoohash_util.hh;/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/libcuckoo_lock_array.hh;/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo/libcuckoo_bucket_container.hh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/install/include/libcuckoo" TYPE FILE FILES
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/city_hasher.hh"
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/cuckoohash_config.hh"
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/cuckoohash_map.hh"
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/cuckoohash_util.hh"
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/libcuckoo_lock_array.hh"
    "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/libcuckoo/libcuckoo_bucket_container.hh"
    )
endif()


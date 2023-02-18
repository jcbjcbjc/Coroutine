# Install script for directory: /test/src/base

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/test/src/build/base/libcommon_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common/base" TYPE FILE FILES
    "/test/src/base/AsyncLogging.h"
    "/test/src/base/Atomic.h"
    "/test/src/base/BlockingQueue.h"
    "/test/src/base/BoundedBlockingQueue.h"
    "/test/src/base/Condition.h"
    "/test/src/base/CountDownLatch.h"
    "/test/src/base/CurrentThread.h"
    "/test/src/base/Date.h"
    "/test/src/base/Exception.h"
    "/test/src/base/FileUtil.h"
    "/test/src/base/GzipFile.h"
    "/test/src/base/LogFile.h"
    "/test/src/base/LogStream.h"
    "/test/src/base/Logging.h"
    "/test/src/base/Mutex.h"
    "/test/src/base/ProcessInfo.h"
    "/test/src/base/Singleton.h"
    "/test/src/base/StringPiece.h"
    "/test/src/base/Thread.h"
    "/test/src/base/ThreadLocal.h"
    "/test/src/base/ThreadLocalSingleton.h"
    "/test/src/base/ThreadPool.h"
    "/test/src/base/TimeZone.h"
    "/test/src/base/Timestamp.h"
    "/test/src/base/Types.h"
    "/test/src/base/WeakCallback.h"
    "/test/src/base/copyable.h"
    "/test/src/base/noncopyable.h"
    )
endif()


# Make file for android-log
# Copyright 2012 by Brandon Edens. All Rights Reserved
# Brandon Edens <brandonedens@gmail.com>
# 3012-08-22

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(android-log)

# Set version info
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)

# Set version string
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Set contact email
SET(APPLICATION_NAME "android-log")
SET(ORGANIZATION_NAME "Android Logcat colorizer")
SET(ORGANIZATION_DOMAIN "http://be4k.com/android-log")
SET(PACKAGE_CONTACT "brandonedens@gmail.com")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -ggdb -Wshadow")

ADD_SUBDIRECTORY(src)

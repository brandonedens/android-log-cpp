# Make file for android-log src.
#
# Copyright 2012 by Brandon Edens. All Rights Reserved
# Brandon Edens <brandonedens@gmail.com>
# 2012-03-22

SET(SOURCES
  device.cc
  main.cc
  )

ADD_EXECUTABLE(android-log
  ${SOURCES}
  )

TARGET_LINK_LIBRARIES(android-log
  boost_regex
  tbb
)

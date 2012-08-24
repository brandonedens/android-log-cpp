
=====================
Android Log Colorizer
=====================


Introduction
============

Congratulations in finding this Android log colorizer. This colorizer is based
upon the data presentation and color scheme found in Jeff Sharkey's `logcat
colorizer`_. In addition, this colorizer is written to simultaneously process
log information from all connected Android devices while also implemented in
C++ for maximum throughput.

.. _`logcat colorizer`: http://jsharkey.org/blog/2009/04/22/modifying-the-android-logcat-stream-for-full-color-debugging/


Quickstart
==========

android-log uses cmake, boost, and tbb to compile. Please install these
libraries onto your computer prior to attempting a build. To compile and
execute use the standard cmake paradigm such as::

  mkdir build
  cmake ~/path/to/android-log
  make
  ./src/android-log


Hacking
=======

This software follows the Google C++ coding standard with the exception that
methods utilize Java style camel case syntax where the first letter in the
leading word in the method name is downcased. Currently C style functions are
using underscore_separated_style, continuing with this style is debatable.

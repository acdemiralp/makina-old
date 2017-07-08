# Find the FreeImage library.
#
# This module defines
#  FREEIMAGEPLUS_FOUND             - True if FREEIMAGE was found.
#  FREEIMAGEPLUS_INCLUDE_DIRS      - Include directories for FREEIMAGE headers.
#  FREEIMAGEPLUS_LIBRARIES         - Libraries for FREEIMAGE.
#
# To specify an additional directory to search, set FREEIMAGEPLUS_ROOT.
#
# Copyright (c) 2010, Ewen Cheslack-Postava
# Based on FindSQLite3.cmake by:
#  Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#  Extended by Siddhartha Chaudhuri, 2008.
#
# Redistribution and use is allowed according to the terms of the BSD license.
#

SET(FREEIMAGEPLUS_FOUND FALSE)
SET(FREEIMAGEPLUS_INCLUDE_DIRS)
SET(FREEIMAGEPLUS_LIBRARIES)

SET(SEARCH_PATHS
  $ENV{ProgramFiles}/freeimage/include
  $ENV{SystemDrive}/freeimage/include
  $ENV{ProgramFiles}/freeimage
  $ENV{SystemDrive}/freeimage
  )
IF(FREEIMAGEPLUS_ROOT)
  SET(SEARCH_PATHS
    ${FREEIMAGEPLUS_ROOT}
    ${FREEIMAGEPLUS_ROOT}/include
    ${SEARCH_PATHS}
    )
ENDIF()

FIND_PATH(FREEIMAGEPLUS_INCLUDE_DIRS
  NAMES FreeImage.h
  PATHS ${SEARCH_PATHS}
  NO_DEFAULT_PATH)
IF(NOT FREEIMAGEPLUS_INCLUDE_DIRS)  # now look in system locations
  FIND_PATH(FREEIMAGEPLUS_INCLUDE_DIRS NAMES FreeImage.h)
ENDIF(NOT FREEIMAGEPLUS_INCLUDE_DIRS)

SET(FREEIMAGEPLUS_LIBRARY_DIRS)
IF(FREEIMAGEPLUS_ROOT)
  SET(FREEIMAGEPLUS_LIBRARY_DIRS ${FREEIMAGEPLUS_ROOT})
  IF(EXISTS "${FREEIMAGEPLUS_ROOT}/lib")
    SET(FREEIMAGEPLUS_LIBRARY_DIRS ${FREEIMAGEPLUS_LIBRARY_DIRS} ${FREEIMAGEPLUS_ROOT}/lib)
  ENDIF()
  IF(EXISTS "${FREEIMAGEPLUS_ROOT}/lib/static")
    SET(FREEIMAGEPLUS_LIBRARY_DIRS ${FREEIMAGEPLUS_LIBRARY_DIRS} ${FREEIMAGEPLUS_ROOT}/lib/static)
  ENDIF()
ENDIF()

# FREEIMAGE
# Without system dirs
FIND_LIBRARY(FREEIMAGEPLUS_LIBRARY
  NAMES freeimage
  PATHS ${FREEIMAGEPLUS_LIBRARY_DIRS}
  NO_DEFAULT_PATH
  )
IF(NOT FREEIMAGEPLUS_LIBRARY)  # now look in system locations
  FIND_LIBRARY(FREEIMAGEPLUS_LIBRARY NAMES freeimage)
ENDIF(NOT FREEIMAGEPLUS_LIBRARY)

SET(FREEIMAGEPLUS_LIBRARIES)
IF(FREEIMAGEPLUS_LIBRARY)
  SET(FREEIMAGEPLUS_LIBRARIES ${FREEIMAGEPLUS_LIBRARY})
ENDIF()

IF(FREEIMAGEPLUS_INCLUDE_DIRS AND FREEIMAGEPLUS_LIBRARIES)
  SET(FREEIMAGEPLUS_FOUND TRUE)
  IF(NOT FREEIMAGEPLUS_FIND_QUIETLY)
    MESSAGE(STATUS "Found FreeImage: headers at ${FREEIMAGEPLUS_INCLUDE_DIRS}, libraries at ${FREEIMAGEPLUS_LIBRARY_DIRS} :: ${FREEIMAGEPLUS_LIBRARIES}")
  ENDIF(NOT FREEIMAGEPLUS_FIND_QUIETLY)
ELSE(FREEIMAGEPLUS_INCLUDE_DIRS AND FREEIMAGEPLUS_LIBRARIES)
  SET(FREEIMAGEPLUS_FOUND FALSE)
  IF(FREEIMAGEPLUS_FIND_REQUIRED)
    MESSAGE(STATUS "FreeImage not found")
  ENDIF(FREEIMAGEPLUS_FIND_REQUIRED)
ENDIF(FREEIMAGEPLUS_INCLUDE_DIRS AND FREEIMAGEPLUS_LIBRARIES)

MARK_AS_ADVANCED(FREEIMAGEPLUS_INCLUDE_DIRS FREEIMAGEPLUS_LIBRARIES)
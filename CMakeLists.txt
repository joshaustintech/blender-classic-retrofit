# $Id$
# ***** BEGIN GPL/BL DUAL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version. The Blender
# Foundation also sells licenses for use in proprietary software under
# the Blender License.  See http://www.blender.org/BL/ for information
# about this.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): Jacques Beaurain.
#
# ***** END GPL/BL DUAL LICENSE BLOCK *****

#-----------------------------------------------------------------------------
# We don't allow in-source builds. This causes no end of troubles because 
# all out-of-source builds will use the CMakeCache.txt file there and even 
# build the libs and objects in it. It will also conflict with the current 
# Makefile system for Blender

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
MESSAGE(FATAL_ERROR "CMake generation for blender is not allowed within the source directory! 
Remove the CMakeCache.txt file and try again from another folder, e.g.: 

   rm CMakeCache.txt 
   cd ..
   mkdir cmake-make 
   cd cmake-make
   cmake -G \"Unix Makefiles\" ../blender
")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(Blender LANGUAGES C CXX)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 98)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

#-----------------------------------------------------------------------------
# Redirect output files

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

#-----------------------------------------------------------------------------
# Set default config options
OPTION(WITH_PLAYER		"Build Player"						OFF)
OPTION(WITH_GAMEENGINE		"Enable Game Engine"					ON)
OPTION(WITH_BULLET		"Enable Bullet (Physics Engine)"			ON)
OPTION(WITH_INTERNATIONAL	"Enable I18N   (International fonts and text)"		ON)
OPTION(WITH_VERSE		"Enable Verse  (http://verse.blender.org)"		OFF)
OPTION(WITH_ELBEEM		"Enable Elbeem (Fluid Simulation)"			ON)
OPTION(WITH_QUICKTIME		"Enable Quicktime Support"				OFF)
OPTION(WITH_OPENEXR		"Enable OpenEXR Support (http://www.openexr.com)"	OFF)
OPTION(WITH_FFMPEG		"Enable FFMPeg Support (http://ffmpeg.mplayerhq.hu/)"	OFF)
OPTION(WITH_OPENAL		"Enable OpenAL Support (http://www.openal.org)"	ON)
OPTION(YESIAMSTUPID		"Enable execution on 64-bit platforms"					OFF)

IF(NOT WITH_GAMEENGINE AND WITH_PLAYER)
  MESSAGE("WARNING: WITH_PLAYER needs WITH_GAMEENGINE")
ENDIF(NOT WITH_GAMEENGINE AND WITH_PLAYER)

# For alternate Python locations the commandline can be used to override detected/default cache settings, e.g:
# On Unix: 
#   cmake -D PYTHON_LIB=/usr/local/lib/python2.3/config/libpython2.3.so -D PYTHON_INC=/usr/local/include/python2.3 -D PYTHON_BINARY=/usr/local/bin/python2.3 -G "Unix Makefiles" ../blender
# On Macs: 
#   cmake -D PYTHON_INC=/System/Library/Frameworks/Python.framework/Versions/2.5/include/python2.5 -D PYTHON_LIBPATH=/System/Library/Frameworks/Python.framework/Versions/2.5/lib/python2.5/config -D PYTHON_BINARY=/System/Library/Frameworks/Python.framework/Versions/2.5/bin/python2.5 -G Xcode ../blender
#
# When changing any of this remember to update the notes in doc/blender-cmake.txt

#-----------------------------------------------------------------------------
# Load some macros.
INCLUDE(CMake/macros.cmake)

#-----------------------------------------------------------------------------
#Platform specifics

IF(UNIX AND NOT APPLE)
  INCLUDE(${CMAKE_ROOT}/Modules/FindOpenAL.cmake)
  IF(OPENAL_FOUND)
    SET(WITH_OPENAL ON)
    SET(OPENAL_LIB ${OPENAL_LIBRARY})
    SET(OPENAL_INC ${OPENAL_INCLUDE_DIR})
  ELSE(OPENAL_FOUND)
    SET(WITH_OPENAL OFF)
  ENDIF(OPENAL_FOUND)

  FIND_LIBRARY(ALUT_LIBRARY
    NAMES alut
    PATHS
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
  )
  IF(ALUT_LIBRARY)
    SET(OPENAL_LIB ${OPENAL_LIB} ${ALUT_LIBRARY})
  ENDIF(ALUT_LIBRARY)

  FIND_LIBRARY(INTL_LIBRARY
    NAMES intl
    PATHS
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
  )
  FIND_LIBRARY(ICONV_LIBRARY
    NAMES iconv
    PATHS
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
  )
  IF(INTL_LIBRARY AND ICONV_LIBRARY)
    SET(GETTEXT_LIB ${INTL_LIBRARY} ${ICONV_LIBRARY})
  ENDIF(INTL_LIBRARY AND ICONV_LIBRARY)
  
  FIND_PATH(FREETYPE_INC
    freetype
    PATHS
    /usr/local/include/freetype2
    /usr/include/freetype2
    /sw/include/freetype2
    /opt/local/include/freetype2
    /opt/csw/include/freetype2
    /opt/include/freetype2
    NO_DEFAULT_PATH
  )
  SET(FREETYPE_LIB freetype)

  INCLUDE(${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  SET(PYTHON_INC "${PYTHON_INCLUDE_PATH}" CACHE STRING "")
  SET(PYTHON_LIB "${PYTHON_LIBRARIES}" CACHE STRING "")
  INCLUDE(${CMAKE_ROOT}/Modules/FindPythonInterp.cmake)
  SET(PYTHON_BINARY ${PYTHON_EXECUTABLE} CACHE STRING "")
  SET(PYTHON_LINKFLAGS "-Xlinker -export-dynamic")

  INCLUDE(${CMAKE_ROOT}/Modules/FindSDL.cmake)
  SET(SDL_INC ${SDL_INCLUDE_DIR})
  SET(SDL_LIB ${SDL_LIBRARY})

  FIND_PATH(OPENEXR_INC
    ImfXdr.h
    PATHS
    /usr/local/include/OpenEXR
    /usr/include/OpenEXR
    /sw/include/OpenEXR
    /opt/local/include/OpenEXR
    /opt/csw/include/OpenEXR
    /opt/include/OpenEXR
  )
  SET(OPENEXR_LIB Half IlmImf Iex Imath)

  SET(FFMPEG /usr)
  SET(FFMPEG_INC ${FFMPEG}/include)
  SET(FFMPEG_LIB avformat avcodec avutil)
  SET(FFMPEG_LIBPATH ${FFMPEG}/lib)

  SET(JPEG_LIB jpeg)

  SET(PNG_LIB png)

  SET(ZLIB_LIB z)

  SET(LLIBS "-lXi -lutil -lc -lm -lpthread -lstdc++")

  SET(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing -DXP_UNIX -Wno-char-subscripts")

  SET(PLATFORM_LINKFLAGS "-pthread")

  INCLUDE_DIRECTORIES(/usr/include /usr/local/include)
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)

  SET(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/windows)

  SET(PYTHON ${LIBDIR}/python)
  SET(PYTHON_VERSION 2.4)
  SET(PYTHON_INC "${PYTHON}/include/python${PYTHON_VERSION}")
  SET(PYTHON_BINARY python)
  SET(PYTHON_LIB python24)
  SET(PYTHON_LIBPATH ${PYTHON}/lib)

  #SET(WITH_OPENAL ON)
  SET(OPENAL ${LIBDIR}/openal)
  SET(OPENAL_INC ${OPENAL}/include ${OPENAL}/include/AL)
  SET(OPENAL_LIB openal_static)
  SET(OPENAL_LIBPATH ${OPENAL}/lib)

  SET(PNG_LIB libpng_st)
  SET(JPEG_LIB libjpeg)

  SET(ZLIB ${LIBDIR}/zlib)
  SET(ZLIB_INC ${ZLIB}/include)
  SET(ZLIB_LIB libz)
  SET(ZLIB_LIBPATH ${ZLIB}/lib)
  
  SET(PTHREADS ${LIBDIR}/pthreads)
  SET(PTHREADS_INC ${PTHREADS}/include)
  SET(PTHREADS_LIB pthreadVC2)
  SET(PTHREADS_LIBPATH ${PTHREADS}/lib)
  
  SET(ICONV ${LIBDIR}/iconv)
  SET(ICONV_INC ${ICONV}/include)
  SET(ICONV_LIB iconv)
  SET(ICONV_LIBPATH ${ICONV}/lib)
  
  SET(GETTEXT ${LIBDIR}/gettext)
  SET(GETTEXT_INC ${GETTEXT}/include)
  SET(GETTEXT_LIB gnu_gettext)
  SET(GETTEXT_LIBPATH ${GETTEXT}/lib)

  SET(FREETYPE ${LIBDIR}/freetype)
  SET(FREETYPE_INC ${FREETYPE}/include ${FREETYPE}/include/freetype2)
  SET(FREETYPE_LIBPATH ${FREETYPE}/lib)
  SET(FREETYPE_LIB freetype2ST)
  
  SET(OPENEXR ${LIBDIR}/openexr)
  SET(OPENEXR_INC ${OPENEXR}/include ${OPENEXR}/include/IlmImf ${OPENEXR}/include/Iex ${OPENEXR}/include/Imath)
  SET(OPENEXR_LIB Iex Half IlmImf Imath IlmThread)
  SET(OPENEXR_LIBPATH ${OPENEXR}/lib_msvc)
  
  SET(QUICKTIME ${LIBDIR}/QTDevWin)
  SET(QUICKTIME_INC ${QUICKTIME}/CIncludes)
  SET(QUICKTIME_LIB qtmlClient)
  SET(QUICKTIME_LIBPATH ${QUICKTIME}/Libraries)

  SET(FFMPEG ${LIBDIR}/ffmpeg)
  SET(FFMPEG_INC ${FFMPEG}/include)
  SET(FFMPEG_LIB avcodec-51 avformat-51 avutil-49)
  SET(FFMPEG_LIBPATH ${FFMPEG}/lib)

  SET(LLIBS kernel32 user32 gdi32 comdlg32 advapi32 shell32 ole32 oleaut32 uuid ws2_32 vfw32 winmm)
  IF(WITH_OPENAL)
    SET(LLIBS ${LLIBS} dxguid)
  ENDIF(WITH_OPENAL)

  SET(CMAKE_CXX_FLAGS_DEBUG "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /D_DEBUG /Od /Gm /EHsc /RTC1 /MTd /W3 /nologo /ZI /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_CXX_FLAGS_RELEASE "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O2 /Ob2 /DNDEBUG /EHsc /MT /W3 /nologo /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_CXX_FLAGS_MINSIZEREL "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O1 /Ob1 /DNDEBUG /EHsc /MT /W3 /nologo /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O2 /Ob1 /DNDEBUG /EHsc /MT /W3 /nologo /Zi /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_C_FLAGS_DEBUG "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /D_DEBUG /Od /Gm /EHsc /RTC1 /MTd /W3 /nologo /ZI /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_C_FLAGS_RELEASE "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O2 /Ob2 /DNDEBUG /EHsc /MT /W3 /nologo /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_C_FLAGS_MINSIZEREL "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O1 /Ob1 /DNDEBUG /EHsc /MT /W3 /nologo /J" CACHE STRING "MSVC MT flags " FORCE)
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO "/D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /wd4800 /wd4244 /wd4305 /O2 /Ob1 /DNDEBUG /EHsc /MT /W3 /nologo /Zi /J" CACHE STRING "MSVC MT flags " FORCE)

  SET(SDL ${LIBDIR}/sdl)
  SET(SDL_INC ${SDL}/include)
  SET(SDL_LIB SDL)
  SET(SDL_LIBPATH ${SDL}/lib)

  SET(PNG "${LIBDIR}/png")
  SET(PNG_INC "${PNG}/include")
  SET(PNG_LIBPATH ${PNG}/lib)

  SET(JPEG "${LIBDIR}/jpeg")
  SET(JPEG_INC "${JPEG}/include")
  SET(JPEG_LIBPATH ${JPEG}/lib)

  SET(TIFF ${LIBDIR}/tiff)
  SET(TIFF_INC ${TIFF}/include)
  
  SET(WINTAB_INC ${LIBDIR}/wintab/include) 

  SET(PLATFORM_LINKFLAGS "/NODEFAULTLIB:libc.lib")
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:libcmt.lib ")
ENDIF(WIN32)

IF(APPLE)
  INCLUDE(CMake/macos.cmake)
ENDIF(APPLE)

#-----------------------------------------------------------------------------
# Common.
SET(VERSE_INC ${CMAKE_SOURCE_DIR}/extern/verse/dist)

SET(FTGL ${CMAKE_SOURCE_DIR}/extern/bFTGL)
SET(FTGL_INC ${FTGL}/include)
SET(FTGL_LIB extern_ftgl)


#-----------------------------------------------------------------------------
# Configure OpenGL.
INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
#-----------------------------------------------------------------------------
# Extra compile flags
IF(WITH_GAMEENGINE)
  SET(PLATFORM_CFLAGS "${PLATFORM_CFLAGS} -DGAMEBLENDER ")
ENDIF(WITH_GAMEENGINE)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PLATFORM_CFLAGS} ")

#-----------------------------------------------------------------------------
# Libraries
FILE(WRITE ${CMAKE_BINARY_DIR}/cmake_blender_libs.txt "")
SUBDIRS(
  intern
  extern
  source
)


#-----------------------------------------------------------------------------
# Blender Application
SUBDIRS(source/creator)

#-----------------------------------------------------------------------------
# Blender Player
IF(WITH_PLAYER)
  SUBDIRS(blenderplayer)
ENDIF(WITH_PLAYER)

# Focused native compatibility checks; GUI checks need WindowServer access.
INCLUDE(CTest)
IF(BUILD_TESTING AND APPLE)
  ADD_EXECUTABLE(ghost_native_test intern/ghost/test/native/GHOST_NativeTest.cpp)
  TARGET_INCLUDE_DIRECTORIES(ghost_native_test PRIVATE intern/ghost intern/string)
  TARGET_LINK_LIBRARIES(ghost_native_test bf_ghost bf_string ${OPENGL_gl_LIBRARY} "-framework Cocoa")
  ADD_TEST(NAME ghost_native COMMAND ghost_native_test)
  ADD_LIBRARY(dynlibtest_plugin MODULE source/blender/blenlib/test/dynlibtest_plugin.c)
  ADD_EXECUTABLE(dynlibtest source/blender/blenlib/test/dynlibtest.c)
  TARGET_INCLUDE_DIRECTORIES(dynlibtest PRIVATE source/blender/blenlib)
  TARGET_LINK_LIBRARIES(dynlibtest bf_blenlib)
  ADD_TEST(NAME dynlib_native COMMAND dynlibtest $<TARGET_FILE:dynlibtest_plugin>)
  ADD_TEST(NAME python_legacy COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/python-smoke.cmake)
  ADD_TEST(NAME tiff_native COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -DTEST_SCRIPT=${CMAKE_SOURCE_DIR}/tools/macos/tests/tiff-smoke.py
    -DTEST_MARKER=NATIVE_TIFF_PASS:
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/python-smoke.cmake)
  ADD_TEST(NAME packed_file_cleanup COMMAND packed_file_check_test)
  ADD_EXECUTABLE(vpaint_color_test source/blender/src/test/vpaint_color_test.c
    source/blender/src/vpaint_color.c)
  TARGET_INCLUDE_DIRECTORIES(vpaint_color_test PRIVATE source/blender/src)
  ADD_TEST(NAME vpaint_color COMMAND vpaint_color_test)
  ADD_EXECUTABLE(zbuf_depth_test source/blender/render/intern/test/zbuf_depth_test.c
    source/blender/render/intern/source/zbuf_depth.c)
  TARGET_INCLUDE_DIRECTORIES(zbuf_depth_test PRIVATE source/blender/render/intern/source)
  ADD_TEST(NAME zbuf_depth COMMAND zbuf_depth_test)
  ADD_TEST(NAME freetype_failure COMMAND freetype_failure_test ${CMAKE_SOURCE_DIR}/release/datafiles/bfont.ttf)
  ADD_TEST(NAME font_error_roundtrip COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/font-error-roundtrip.cmake)
  ADD_TEST(NAME name_boundary COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/name-boundary.cmake)
  ADD_TEST(NAME packed_file_roundtrip COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/packed-file-roundtrip.cmake)
  ADD_TEST(NAME python_traceback_lifetime COMMAND ${CMAKE_COMMAND}
    -DBLENDER_EXECUTABLE=$<TARGET_FILE:blender> -DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
    -P ${CMAKE_SOURCE_DIR}/tools/macos/tests/python-traceback-lifetime.cmake)
ENDIF()

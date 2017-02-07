cmake_minimum_required(VERSION 2.8)

project(STree)

find_library(ICONV_LIBRARY NAMES iconv)

find_path(ICONV_INCLUDES NAMES iconv.h)
if(ICONV_INCLUDES)
  message("iconv.h found at " ${ICONV_INCLUDES})
else(ICONV_INCLUDES)
  message(FATAL_ERROR "iconv.h not found")
endif(ICONV_INCLUDES)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "includes/*.h")

include_directories("includes/" ${ICONV_INCLUDES})

add_library(STree SHARED ${SOURCE_FILES})

if(ICONV_LIBRARY)
  message("libiconv found at " ${ICONV_LIBRARY})
  target_link_libraries(STree ${ICONV_LIBRARY})
else(ICONV_LIBRARY)
  message("libiconv not found")
endif(ICONV_LIBRARY)

set_property(TARGET STree PROPERTY CXX_STANDARD 14)

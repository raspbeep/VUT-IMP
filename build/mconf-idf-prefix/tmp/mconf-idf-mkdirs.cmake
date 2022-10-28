# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/pakratoc/esp/esp-idf/tools/kconfig"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/kconfig_bin"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/tmp"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/src/mconf-idf-stamp"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/src"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/src/mconf-idf-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/src/mconf-idf-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/mconf-idf-prefix/src/mconf-idf-stamp${cfgdir}") # cfgdir has leading slash
endif()

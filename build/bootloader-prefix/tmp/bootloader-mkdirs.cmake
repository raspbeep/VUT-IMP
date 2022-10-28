# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/pakratoc/esp/esp-idf/components/bootloader/subproject"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/tmp"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/src"
  "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/pakratoc/Documents/PlatformIO/Projects/esp32/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

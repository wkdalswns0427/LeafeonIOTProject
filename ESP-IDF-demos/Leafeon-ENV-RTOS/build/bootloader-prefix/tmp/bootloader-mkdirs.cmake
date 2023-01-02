# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/master/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix/tmp"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix/src"
  "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/master/Desktop/Leafeon-ENV-RTOS/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()

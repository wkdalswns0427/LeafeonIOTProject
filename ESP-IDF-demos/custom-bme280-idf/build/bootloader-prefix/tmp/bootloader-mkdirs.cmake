# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/pawpa/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/tmp"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/src"
  "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/pawpa/Desktop/BME280_ESP_IDF/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

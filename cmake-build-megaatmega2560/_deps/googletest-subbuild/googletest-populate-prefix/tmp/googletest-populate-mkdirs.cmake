# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-src"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-build"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

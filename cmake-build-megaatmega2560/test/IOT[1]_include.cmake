if(EXISTS "C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/test/IOT[1]_tests.cmake")
  include("C:/Users/45552/Desktop/VIA_UC/Semester4/SEP4/IOT-LoRaWAN-receive/cmake-build-megaatmega2560/test/IOT[1]_tests.cmake")
else()
  add_test(IOT_NOT_BUILT IOT_NOT_BUILT)
endif()

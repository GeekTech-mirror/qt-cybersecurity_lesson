if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

PROJECT(pcap CXX)

add_subdirectory(libpcap)


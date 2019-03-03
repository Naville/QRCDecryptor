cmake_minimum_required(VERSION 3.6)
project(QQMusicTools)
set(CMAKE_CXX_STANDARD 11)
find_package(ZLIB REQUIRED)
include_directories(AFTER SYSTEM ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/gzip-hpp/include)
add_library(QMTLyrics
  ${CMAKE_CURRENT_LIST_DIR}/lib/QMLyrics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/lib/DES.cpp
)
add_executable(QQMusicDecryptor
  ${CMAKE_CURRENT_LIST_DIR}/tools/QQMusicDecryptor.cpp
)
target_link_libraries(QMTLyrics ${ZLIB_LIBRARIES})
target_link_libraries(QQMusicDecryptor QMTLyrics)

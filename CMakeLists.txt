project( sap CXX )

cmake_minimum_required( VERSION 2.8.11 )

add_definitions( -std=c++11 )

file( GLOB sap_SRCS "*.cxx" )

add_executable( sap ${sap_SRCS} )


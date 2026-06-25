// MIT License. Copyright 2017-2018 Norwegian University of Technology (NTNU)
#ifndef FMI4CPP_TYPES_HPP
#define FMI4CPP_TYPES_HPP
namespace fmi4cpp {
typedef void* fmi4cppComponent; typedef void* fmi4cppFMUstate;
typedef unsigned int fmi4cppValueReference;
typedef double fmi4cppReal; typedef int fmi4cppInteger; typedef int fmi4cppBoolean;
typedef char fmi4cppChar; typedef const fmi4cppChar* fmi4cppString; typedef char fmi4cppByte;
} // namespace fmi4cpp
#endif //FMI4CPP_TYPES_HPP

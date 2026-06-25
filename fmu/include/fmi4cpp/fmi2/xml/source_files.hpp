// MIT License. Copyright 2017-2018 Norwegian University of Technology (NTNU)
#ifndef FMI4CPP_SOURCEFILES_HPP
#define FMI4CPP_SOURCEFILES_HPP
#include <string>
#include <vector>
namespace fmi4cpp::fmi2 {
struct source_file { std::string name; };
typedef std::vector<source_file> source_files;
} // namespace fmi4cpp::fmi2
#endif //FMI4CPP_SOURCEFILES_HPP

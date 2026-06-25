// MIT License. Copyright 2017-2018 Norwegian University of Technology (NTNU)
#ifndef FMI4CPP_MODELDESCRIPTIONPARSER_HPP
#define FMI4CPP_MODELDESCRIPTIONPARSER_HPP
#include <fmi4cpp/fmi2/xml/model_description.hpp>
#include <memory>
#include <string>
namespace fmi4cpp::fmi2 {
std::unique_ptr<const model_description> parse_model_description(const std::string& fileName);
}
#endif //FMI4CPP_MODELDESCRIPTIONPARSER_HPP

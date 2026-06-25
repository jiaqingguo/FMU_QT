// MIT License. Copyright 2017-2018 Norwegian University of Technology (NTNU)
#ifndef FMI4CPP_FMURESOURCE_HPP
#define FMI4CPP_FMURESOURCE_HPP
#include <fmi4cpp/fs_portability.hpp>
#include <string>
namespace fmi4cpp {
class fmu_resource {
private:
    const fs::path path_;
public:
    explicit fmu_resource(fs::path path); ~fmu_resource();
    [[nodiscard]] std::string resource_path() const; [[nodiscard]] std::string model_description_path() const;
    [[nodiscard]] std::string absolute_library_path(const std::string& modelIdentifier) const;
    [[nodiscard]] std::string get_model_description_xml() const;
};
} // namespace fmi4cpp
#endif //FMI4CPP_FMURESOURCE_HPP

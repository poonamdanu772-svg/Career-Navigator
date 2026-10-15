#include "Career.h"
#include <utility>

Career::Career(std::string name, std::string education,
               std::vector<std::string> interests,
               std::vector<std::string> skills)
    : name(std::move(name)),
      requiredEducation(std::move(education)),
      interests(std::move(interests)),
      requiredSkills(std::move(skills)) {
}

std::string Career::getName() const {
    return name;
}

std::string Career::getRequiredEducation() const {
    return requiredEducation;
}

const std::vector<std::string>& Career::getInterests() const {
    return interests;
}

const std::vector<std::string>& Career::getRequiredSkills() const {
    return requiredSkills;
}
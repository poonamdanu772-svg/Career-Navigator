
#include "Career.h"

Career::Career(std::string name, std::string description,
               std::string education,
               std::vector<std::string> interests,
               std::vector<std::string> skills)
    : name(name),
      description(description),
      requiredEducation(education),
      interests(interests),
      requiredSkills(skills)
{
}

std::string Career::getName() const
{
    return name;
}

std::string Career::getDescription() const
{
    return description;
}

std::string Career::getRequiredEducation() const
{
    return requiredEducation;
}

const std::vector<std::string>& Career::getInterests() const
{
    return interests;
}

const std::vector<std::string>& Career::getRequiredSkills() const
{
    return requiredSkills;
}

#ifndef CAREER_H
#define CAREER_H

#include <string>
#include <vector>

class Career {
private:
    std::string name;
    std::string requiredEducation;
    std::vector<std::string> interests;
    std::vector<std::string> requiredSkills;

public:
    Career(std::string name, std::string education,
           std::vector<std::string> interests,
           std::vector<std::string> skills);

    std::string getName() const;
    std::string getRequiredEducation() const;

    const std::vector<std::string>& getInterests() const;
    const std::vector<std::string>& getRequiredSkills() const;
};

#endif
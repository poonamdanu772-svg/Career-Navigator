#ifndef CAREER_DATABASE_H
#define CAREER_DATABASE_H

#include <string>
#include <vector>
#include "Career.h"

class CareerDatabase {
private:
    std::vector<Career> careers;

public:
    bool loadFromFile(const std::string& filename);
    void addCareer(const Career& career);

    const std::vector<Career>& getAllCareers() const;
};

#endif

#ifndef ROADMAP_H
#define ROADMAP_H

#include <string>
#include <vector>
#include "Step.h"

using namespace std;

class Roadmap
{
private:
    string careerName;
    string pathType;
    vector<Step> steps;

public:
    Roadmap(const string& careerName, const string& pathType);

    string getCareerName() const;
    string getPathType() const;

    void addStep(const Step& step);
    const vector<Step>& getSteps() const;

    bool loadStepsFromCSV(const string& filename);
};

#endif

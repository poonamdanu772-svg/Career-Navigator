#ifndef ROADMAP.H
#define ROADMAP.H

#include <string>
#include <vector>
#include "Step.h"

class Roadmap {
    private:
     vector<Steps> steps;
     string CareerName;
    public:
    
     Roadmap(string name);
     string getCareerName();
     void addStep(Steps step);
     vector<Steps> getSteps();
};

#endif
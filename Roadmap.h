#ifndef ROADMAP_H
#define ROADMAP_H

#include <string>
#include <vector>
#include "Step.h"

using namespace std;

class Roadmap {
    private:
     vector<Step> steps;
     string CareerName;
    public:
    
     Roadmap(string name);
     string getCareerName();
     void addStep(Step step);
     vector<Step> getSteps();

     bool loadStepsFromCSV(const string& filename);
};

#endif
#include "Step.h"
#include "Roadmap.h"

Roadmap::Roadmap(string CareerName){
  this->CareerName = CareerName;  
}

string Roadmap::getCareerName(){
  return CareerName;
}

vector<Step> Roadmap::getSteps() {
    return steps;
}

void Roadmap::addStep(Step step) {
    steps.push_back(step);
}

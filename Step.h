#ifndef STEP_H
#define STEP_H

#include <string>
using namespace std;

class Step {
private:
    int snum;
    string title;
    string description;

public:
    Step(int snum, string title, string description);

    int getSnum() const ;
    string getTitle() const ;
    string getDescription() const ;
};

#endif
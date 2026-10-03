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

    int getSnum();
    string getTitle();
    string getDescription();
};

#endif
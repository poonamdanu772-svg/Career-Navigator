
#ifndef CAREERPATH_H
#define CAREERPATH_H

#include <iostream>
#include <string>
using namespace std;

class CareerPath
{
public:
    virtual void displayPath() const = 0;

    virtual ~CareerPath() {}
};

#endif

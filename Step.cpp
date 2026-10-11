#include "Step.h"

Step ::Step(int snum, string title, string description) {
    this->snum = snum;
    this->title = title;
    this->description = description;
}

int Step::getSnum() const
{
    return snum;
}

string Step::getTitle() const
{
    return title;
}

string Step::getDescription() const
{
    return description;
}



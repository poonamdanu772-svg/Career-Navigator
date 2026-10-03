#include "Step.h"

Step ::Step(int snum, string title, string description) {
    this->snum = snum;
    this->title = title;
    this->description = description;
}

int Step::getSnum() {
    return snum;
}

string Step::getTitle() {
    return title;
}

string Step::getDescription() {
    return description;
}



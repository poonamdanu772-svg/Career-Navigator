
#ifndef DEGREECAREERPATH_H
#define DEGREECAREERPATH_H

#include "CareerPath.h"
#include "Roadmap.h"

class DegreeCareerPath : public CareerPath
{
private:
    string careerName;

public:
    DegreeCareerPath(const string& careerName)
        : careerName(careerName)
    {
    }

    void displayPath() const override
    {
        Roadmap roadmap(careerName, "Degree");

        if (!roadmap.loadStepsFromCSV("roadmap_data.csv"))
        {
            cout << "No degree-based roadmap found for "
                 << careerName << ".\n";
            return;
        }

        cout << "\n=== Degree-Based Roadmap: "
             << careerName << " ===\n";

        for (const Step& step : roadmap.getSteps())
        {
            cout << "\nStep " << step.getSnum()
                 << ": " << step.getTitle() << "\n";
            cout << step.getDescription() << "\n";
        }
    }
};

#endif


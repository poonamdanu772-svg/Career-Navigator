
#ifndef SKILLBASEDCAREERPATH_H
#define SKILLBASEDCAREERPATH_H

#include "CareerPath.h"
#include "Roadmap.h"

class SkillBasedCareerPath : public CareerPath
{
private:
    string careerName;

public:
    SkillBasedCareerPath(const string& careerName)
        : careerName(careerName)
    {
    }

    void displayPath() const override
    {
        Roadmap roadmap(careerName, "Skill");

        if (!roadmap.loadStepsFromCSV("roadmap_data.csv"))
        {
            cout << "No skill-based roadmap found for "
                 << careerName << ".\n";
            return;
        }

        cout << "\n=== Skill-Based Roadmap: "
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



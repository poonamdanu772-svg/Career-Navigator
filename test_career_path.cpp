
#include "DegreeCareerPath.h"
#include "SkillBasedCareerPath.h"

int main()
{
    DegreeCareerPath degree;
    SkillBasedCareerPath skills;

    CareerPath* path;

    cout << "Choose a career path:\n";
    cout << "1. Degree-based\n";
    cout << "2. Skill-based\n";

    int choice;
    cin >> choice;

    if (choice == 1)
        path = &degree;
    else if (choice == 2)
        path = &skills;
    else
    {
        cout << "Invalid choice!\n";
        return 0;
    }

    path->displayPath();

    return 0;
}


#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

#include "User.h"
#include "Career.h"
#include "CareerDatabase.h"
#include "RecommendationEngine.h"
#include "Roadmap.h"
#include "Step.h"
#include "DegreeCareerPath.h"
#include "SkillBasedCareerPath.h"

using namespace std;

void demonstrateCareerPaths(const string& careerName)
{
    DegreeCareerPath degree(careerName);
    SkillBasedCareerPath skills(careerName);

    CareerPath* path = nullptr;

    int choice;

    cout << "\n=== Explore Career Preparation Paths ===\n";
    cout << "Career: " << careerName << "\n";
    cout << "1. Degree-Based Path\n";
    cout << "2. Skill-Based Path\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice))
    {
        cout << "Invalid input.\n";
        return;
    }

    if (choice == 1)
        path = &degree;
    else if (choice == 2)
        path = &skills;
    else
    {
        cout << "Invalid choice!\n";
        return;
    }

    path->displayPath();
}


int main()
{
    cout << "=====================================\n";
    cout << "        CAREER NAVIGATOR\n";
    cout << "=====================================\n\n";

    // 1. Collect the user profile
    string name, education, stream, interests, skills, careerGoal;
    int age;
    double budget;

    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your age: ";
    if (!(cin >> age) || age < User::MIN_AGE || age > User::MAX_AGE)
    {
        cout << "Invalid age.\n";
        return 1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your education level: ";
    getline(cin, education);

    cout << "Enter your academic stream: ";
    getline(cin, stream);

    cout << "Enter interests (comma-separated): ";
    getline(cin, interests);

    cout << "Enter skills (comma-separated): ";
    getline(cin, skills);

    cout << "Enter your education/training budget: ";
    if (!(cin >> budget) || budget < 0)
    {
        cout << "Invalid budget.\n";
        return 1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your long-term career goal: ";
    getline(cin, careerGoal);

    User user;

    if (!user.setName(name) ||
        !user.setAge(age) ||
        !user.setEducation(education) ||
        !user.setStream(stream) ||
        !user.setInterests(interests) ||
        !user.setSkills(skills) ||
        !user.setBudget(budget) ||
        !user.setCareerGoal(careerGoal))
    {
        cout << "Invalid profile details.\n";
        return 1;
    }

    cout << "\nProfile created successfully!\n";

    // 2. Load careers from the CSV file
    CareerDatabase database;

    if (!database.loadFromFile("careers.csv"))
    {
        cout << "Could not load careers.csv.\n";
        return 1;
    }

    const vector<Career>& careers = database.getAllCareers();

    if (careers.empty())
    {
        cout << "No careers are available.\n";
        return 1;
    }

    
    // 3. Select a recommendation strategy
    BalancedRecommendationStrategy balancedStrategy;
    InterestFocusedStrategy interestStrategy;

    const RecommendationStrategy* selectedStrategy = nullptr;

    int strategyChoice;

    cout << "\n=== Choose Recommendation Strategy ===\n";
    cout << "1. Balanced (Education 20%, Interests 40%, Skills 40%)\n";
    cout << "2. Interest-Focused (Education 15%, Interests 60%, Skills 25%)\n";
    cout << "Enter your choice: ";

    if (!(cin >> strategyChoice))
    {
        cout << "Invalid input.\n";
        return 1;
    }

    if (strategyChoice == 1)
        selectedStrategy = &balancedStrategy;
    else if (strategyChoice == 2)
        selectedStrategy = &interestStrategy;
    else
    {
        cout << "Invalid strategy selection.\n";
        return 1;
    }

    // 4. Generate recommendations using the selected strategy
    RecommendationEngine engine;

    vector<Recommendation> recommendations =
        engine.recommendCareers(user, careers, *selectedStrategy);

    if (recommendations.empty())
    {
        cout << "No career recommendations available.\n";
        return 0;
    }


    cout << "\n========== CAREER RECOMMENDATIONS ==========\n";

    cout << fixed << setprecision(1);

    for (size_t i = 0; i < recommendations.size(); i++)
    {
        cout << i + 1 << ". "
             << recommendations[i].career.getName()
             << " - "
             << recommendations[i].score * 100
             << "% match\n";
    }
    
    // 4. Navigate between recommended careers and preparation paths

    while (true)
    {
        cout << "\n========== RECOMMENDED CAREERS ==========\n";

        for (size_t i = 0; i < recommendations.size(); i++)
        {
            cout << i + 1 << ". "
                 << recommendations[i].career.getName()
                 << " - "
                 << recommendations[i].score * 100
                 << "% match\n";
        }

        cout << "0. Exit Career Navigator\n";
        cout << "\nEnter the number of a career to explore: ";

        int careerChoice;

        if (!(cin >> careerChoice))
        {
            cout << "Invalid input. Exiting.\n";
            return 1;
        }

        if (careerChoice == 0)
            break;

        if (careerChoice < 1 ||
            careerChoice > static_cast<int>(recommendations.size()))
        {
            cout << "Invalid career selection.\n";
            continue;
        }

        const Career& selectedCareer =
            recommendations[careerChoice - 1].career;

        cout << "\nSelected career: "
             << selectedCareer.getName() << "\n";

        cout << "Education requirement: "
             << selectedCareer.getRequiredEducation() << "\n";

        // Allow repeated exploration of paths for this career.
        while (true)
        {
            cout << "\n=== Career Preparation Paths ===\n";
            cout << "Career: " << selectedCareer.getName() << "\n";
            cout << "1. Degree-Based Path\n";
            cout << "2. Skill-Based Path\n";
            cout << "3. Choose another career\n";
            cout << "4. Exit Career Navigator\n";
            cout << "Enter your choice: ";

            int pathChoice;

            if (!(cin >> pathChoice))
            {
                cout << "Invalid input. Exiting.\n";
                return 1;
            }

            if (pathChoice == 3)
                break;

            if (pathChoice == 4)
            {
                cout << "\nThank you for using Career Navigator!\n";
                return 0;
            }

            if (pathChoice != 1 && pathChoice != 2)
            {
                cout << "Invalid choice. Try again.\n";
                continue;
            }

            CareerPath* path = nullptr;

            if (pathChoice == 1)
                path = new DegreeCareerPath(selectedCareer.getName());
            else
                path = new SkillBasedCareerPath(selectedCareer.getName());

            path->displayPath();

            delete path;

            cout << "\nWhat would you like to do next?\n";
            cout << "1. Explore the other preparation path\n";
            cout << "2. Explore another career\n";
            cout << "3. Return to this career's path menu\n";
            cout << "4. Exit Career Navigator\n";
            cout << "Enter your choice: ";

            int nextChoice;

            if (!(cin >> nextChoice))
            {
                cout << "Invalid input. Exiting.\n";
                return 1;
            }

            if (nextChoice == 1)
            {
                // Switch to the opposite path.
                pathChoice = (pathChoice == 1) ? 2 : 1;

                CareerPath* nextPath = nullptr;

                if (pathChoice == 1)
                    nextPath = new DegreeCareerPath(selectedCareer.getName());
                else
                    nextPath = new SkillBasedCareerPath(selectedCareer.getName());

                nextPath->displayPath();
                delete nextPath;
            }
            else if (nextChoice == 2)
            {
                break;
            }
            else if (nextChoice == 3)
            {
                continue;
            }
            else if (nextChoice == 4)
            {
                cout << "\nThank you for using Career Navigator!\n";
                return 0;
            }
            else
            {
                cout << "Invalid choice. Returning to the path menu.\n";
            }
        }
    }

    cout << "\nThank you for using Career Navigator!\n";
    return 0;
}

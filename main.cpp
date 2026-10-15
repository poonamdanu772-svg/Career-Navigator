
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "User.h"
#include "Career.h"
#include "CareerDatabase.h"
#include "RecommendationEngine.h"
#include "Roadmap.h"
#include "CareerPath.h"
#include "DegreeCareerPath.h"
#include "SkillBasedCareerPath.h"

using namespace std;

namespace
{
    string normalizeText(string text)
    {
        for (char& ch : text)
        {
            ch = static_cast<char>(
                tolower(static_cast<unsigned char>(ch)));
        }

        size_t first = text.find_first_not_of(" \t\r\n");

        if (first == string::npos)
            return "";

        size_t last = text.find_last_not_of(" \t\r\n");

        return text.substr(first, last - first + 1);
    }

    bool readNumber(double& number)
    {
        if (!(cin >> number))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }

    bool readChoice(int& choice)
    {
        double input;

        if (!readNumber(input) ||
            input != static_cast<int>(input))
        {
            return false;
        }

        choice = static_cast<int>(input);
        return true;
    }
}

int main()
{
    cout << "========================================\n";
    cout << "           CAREER NAVIGATOR\n";
    cout << "========================================\n\n";

    // 1. Collect user profile.
    string name, education, stream, interests, skills, careerGoal;
    int age;
    double ageInput;
    double budget;

    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your age: ";

    if (!readNumber(ageInput) ||
        ageInput < User::MIN_AGE ||
        ageInput > User::MAX_AGE ||
        ageInput != static_cast<int>(ageInput))
    {
        cout << "Invalid age. Please enter a valid whole number.\n";
        return 1;
    }

    age = static_cast<int>(ageInput);

    cout << "Enter your education level: ";
    getline(cin, education);

    cout << "Enter your academic stream: ";
    getline(cin, stream);

    cout << "Enter interests separated by commas: ";
    getline(cin, interests);

    cout << "Enter skills separated by commas: ";
    getline(cin, skills);

    cout << "Enter your education/training budget: ";

    if (!readNumber(budget) || budget < 0)
    {
        cout << "Invalid budget. Enter a non-negative number.\n";
        return 1;
    }

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
        cout << "Invalid profile. Please check your entries.\n";
        return 1;
    }

    cout << "\nProfile created successfully!\n";
    user.displayProfile();

    // 2. Load careers from CSV.
    CareerDatabase database;

    if (!database.loadFromFile("careers.csv"))
    {
        cout << "Could not load careers.csv. "
             << "Place it beside the program and try again.\n";
        return 1;
    }

    const vector<Career>& careers = database.getAllCareers();

    if (careers.empty())
    {
        cout << "No careers are available.\n";
        return 1;
    }

    // 3. Select recommendation strategy.
    cout << "\n========== RECOMMENDATION STYLE ==========\n";
    cout << "1. Balanced (Education 20%, Interests 40%, Skills 40%)\n";
    cout << "2. Interest-focused (Education 15%, Interests 60%, Skills 25%)\n";
    cout << "Enter your choice: ";

    int strategyChoice;

    if (!readChoice(strategyChoice) ||
        (strategyChoice != 1 && strategyChoice != 2))
    {
        cout << "Invalid strategy choice.\n";
        return 1;
    }

    BalancedRecommendationStrategy balancedStrategy;
    InterestFocusedStrategy interestStrategy;

    // Runtime polymorphism: use the selected strategy through its base class.
    const RecommendationStrategy* selectedStrategy = &balancedStrategy;

    if (strategyChoice == 2)
        selectedStrategy = &interestStrategy;

    // 4. Generate and rank recommendations.
    RecommendationEngine engine;

    vector<Recommendation> recommendations =
        engine.recommendCareers(user, careers, *selectedStrategy);

    if (recommendations.empty())
    {
        cout << "\nNo career matches were found. "
             << "Try adding relevant interests or skills.\n";
        return 0;
    }

    // Remove duplicate career names, ignoring case.
    vector<Recommendation> uniqueRecommendations;

    for (const Recommendation& item : recommendations)
    {
        bool duplicate = false;

        for (const Recommendation& existing : uniqueRecommendations)
        {
            if (normalizeText(existing.career.getName()) ==
                normalizeText(item.career.getName()))
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate)
            uniqueRecommendations.push_back(item);
    }

    // Display only the top five careers.
    const size_t MAX_RECOMMENDATIONS = 5;

    if (uniqueRecommendations.size() > MAX_RECOMMENDATIONS)
    {
       uniqueRecommendations.erase(
         uniqueRecommendations.begin() + MAX_RECOMMENDATIONS,
         uniqueRecommendations.end()
       );
    }

    cout << fixed << setprecision(1);

    cout << "\n========================================\n";
    cout << "          YOUR TOP CAREER MATCHES\n";
    cout << "========================================\n";


    for (size_t i = 0; i < uniqueRecommendations.size(); i++)
    {
       cout << "\n" << i + 1 << ". "
           << uniqueRecommendations[i].career.getName()
           << " - "
           << uniqueRecommendations[i].score * 100
           << "% match\n";

        cout << "   Description: "
           << uniqueRecommendations[i].career.getDescription()
           << "\n";
    }


    // 5. Keep allowing the user to explore recommended careers.
    while (true)
    {
        cout << "\n========== EXPLORE CAREERS ==========\n";
        cout << "Enter a career name from the list above.\n";
        cout << "Type 'list' to display recommendations again.\n";
        cout << "Type 'exit' to quit.\n";
        cout << "Career name: ";

        string selectedName;
        getline(cin, selectedName);

        string selection = normalizeText(selectedName);

        if (selection == "exit" || selection == "quit")
            break;

        if (selection == "list")
        {
              
          for (size_t i = 0; i < uniqueRecommendations.size(); i++)
          {
            cout << "\n" << i + 1 << ". "
                 << uniqueRecommendations[i].career.getName()
                 << " - "
                 << uniqueRecommendations[i].score * 100
                 << "% match\n";

            cout << "   Description: "
                 << uniqueRecommendations[i].career.getDescription()
                 << "\n";
          }


            continue;
        }

        const Career* selectedCareer = nullptr;

        for (const Recommendation& item : uniqueRecommendations)
        {
            if (normalizeText(item.career.getName()) == selection)
            {
                selectedCareer = &item.career;
                break;
            }
        }

        if (selectedCareer == nullptr)
        {
            cout << "Career not found in your recommended list. "
                 << "Please enter a career name from the list.\n";
            continue;
        }

        cout << "\nSelected career: "
             << selectedCareer->getName() << "\n";
        cout << "Description: "
             << selectedCareer->getDescription() << "\n";
        cout << "Education requirement: "
             << selectedCareer->getRequiredEducation() << "\n";

        // 6. Allow the user to explore both paths for this career.
        int nextPathChoice = 0;
        bool chooseAnotherCareer = false;

        while (!chooseAnotherCareer)
        {
            int pathChoice = nextPathChoice;
            nextPathChoice = 0;

            if (pathChoice == 0)
            {
                cout << "\n========== CAREER PREPARATION PATHS ==========\n";
                cout << "Career: " << selectedCareer->getName() << "\n";
                cout << "1. Degree-Based Path\n";
                cout << "2. Skill-Based Path\n";
                cout << "3. Choose another recommended career\n";
                cout << "4. Exit Career Navigator\n";
                cout << "Enter your choice: ";

                if (!readChoice(pathChoice))
                {
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }

                if (pathChoice == 3)
                {
                    chooseAnotherCareer = true;
                    continue;
                }

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
            }

            // Runtime polymorphism: call the chosen path implementation.
            DegreeCareerPath degreePath(selectedCareer->getName());
            SkillBasedCareerPath skillPath(selectedCareer->getName());

            CareerPath* path = nullptr;

            if (pathChoice == 1)
                path = &degreePath;
            else
                path = &skillPath;

            path->displayPath();

            // 7. Decide what to explore after displaying the roadmap.
            cout << "\n========== WHAT NEXT? ==========\n";
            cout << "1. Explore the other path for this career\n";
            cout << "2. Explore another recommended career\n";
            cout << "3. Return to this career's path menu\n";
            cout << "4. Exit Career Navigator\n";
            cout << "Enter your choice: ";

            int nextChoice;

            if (!readChoice(nextChoice))
            {
                cout << "Invalid input. Returning to the path menu.\n";
                continue;
            }

            if (nextChoice == 1)
            {
                // Switch Degree <-> Skill for the same career.
                nextPathChoice = (pathChoice == 1) ? 2 : 1;
            }
            else if (nextChoice == 2)
            {
                chooseAnotherCareer = true;
            }
            else if (nextChoice == 3)
            {
                nextPathChoice = 0;
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

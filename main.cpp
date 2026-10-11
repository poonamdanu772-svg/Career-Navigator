
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
#include "Step.h"

using namespace std;

string normalizeText(string text)
{
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char ch)
        {
            return static_cast<char>(tolower(ch));
        });

    size_t start = text.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = text.find_last_not_of(" \t\r\n");

    return text.substr(start, end - start + 1);
}

int main()
{
    cout << "========================================\n";
    cout << "           CAREER NAVIGATOR\n";
    cout << "========================================\n\n";

    // 1. Collect user information
    string name, education, stream;
    string interests, skills, careerGoal;
    int age;
    double budget;

    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your age: ";

    if (!(cin >> age) ||
        age < User::MIN_AGE ||
        age > User::MAX_AGE)
    {
        cout << "Invalid age.\n";
        return 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your education level: ";
    getline(cin, education);

    cout << "Enter your academic stream: ";
    getline(cin, stream);

    cout << "Enter your interests (comma-separated): ";
    getline(cin, interests);

    cout << "Enter your skills (comma-separated): ";
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

    // 2. Create the user profile
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
        cout << "Invalid profile information.\n";
        return 1;
    }

    cout << "\nProfile created successfully!\n";

    // 3. Load the career database
    CareerDatabase database;

    if (!database.loadFromFile("careers.csv"))
    {
        cout << "Unable to load careers.csv.\n";
        return 1;
    }

    const vector<Career>& careers = database.getAllCareers();

    if (careers.empty())
    {
        cout << "The career database is empty.\n";
        return 1;
    }

    // 4. Generate recommendations
    RecommendationEngine engine;

    vector<Recommendation> recommendations =
        engine.recommendCareers(user, careers);

    if (recommendations.empty())
    {
        cout << "No recommendations could be generated.\n";
        return 0;
    }

    // 5. Sort recommendations by score
    sort(recommendations.begin(), recommendations.end(),
        [](const Recommendation& a, const Recommendation& b)
        {
            return a.score > b.score;
        });

    // 6. Remove duplicate career names
    vector<Recommendation> uniqueRecommendations;

    for (const auto& recommendation : recommendations)
    {
        string currentName =
            normalizeText(recommendation.career.getName());

        if (currentName.empty())
            continue;

        bool duplicate = false;

        for (const auto& existing : uniqueRecommendations)
        {
            if (normalizeText(existing.career.getName()) == currentName)
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate)
            uniqueRecommendations.push_back(recommendation);
    }

    recommendations = uniqueRecommendations;

    if (recommendations.empty())
    {
        cout << "No valid career recommendations found.\n";
        return 0;
    }

    // 7. Display the five highest-ranked careers
    const size_t MAX_RECOMMENDATIONS = 5;

    if (recommendations.size() > MAX_RECOMMENDATIONS)
    {
        recommendations.resize(MAX_RECOMMENDATIONS);
    }

    cout << fixed << setprecision(1);

    cout << "\n========================================\n";
    cout << "       YOUR TOP CAREER MATCHES\n";
    cout << "========================================\n";

    for (size_t i = 0; i < recommendations.size(); ++i)
    {
        cout << i + 1 << ". "
             << recommendations[i].career.getName()
             << " - "
             << recommendations[i].score * 100
             << "% match\n";
    }

    cout << "\nTotal recommended careers: "
         << recommendations.size() << "\n";

    // 8. Allow repeated roadmap exploration
    while (true)
    {
        string selectedName;

        cout << "\nEnter a recommended career name to view its roadmap";
        cout << "\n(Type exit to quit): ";

        getline(cin, selectedName);

        string normalizedSelection = normalizeText(selectedName);

        if (normalizedSelection == "exit" ||
            normalizedSelection == "quit")
        {
            break;
        }

        const Career* selectedCareer = nullptr;

        for (const auto& recommendation : recommendations)
        {
            if (normalizeText(recommendation.career.getName())
                == normalizedSelection)
            {
                selectedCareer = &recommendation.career;
                break;
            }
        }

        if (selectedCareer == nullptr)
        {
            cout << "Career not found in your recommended list.\n";
            cout << "Please enter one of the displayed career names.\n";
            continue;
        }

        // 9. Load the selected career roadmap
        Roadmap roadmap(selectedCareer->getName());

        if (!roadmap.loadStepsFromCSV("roadmap_data.csv"))
        {
            cout << "\nNo roadmap is available for "
                 << selectedCareer->getName() << ".\n";
        }
        else
        {
            cout << "\n========================================\n";
            cout << "ROADMAP: "
                 << selectedCareer->getName() << "\n";
            cout << "========================================\n";

            for (const Step& step : roadmap.getSteps())
            {
                cout << "\nStep " << step.getSnum()
                     << ": " << step.getTitle() << "\n";

                cout << step.getDescription() << "\n";
            }

            cout << "\n========================================\n";
        }

        // 10. Ask whether the user wants another roadmap
        string answer;

        cout << "\nWould you like to explore another recommended career? (yes/no): ";
        getline(cin, answer);

        answer = normalizeText(answer);

        if (answer != "yes" && answer != "y")
            break;
    }

    cout << "\nThank you for using Career Navigator!\n";

    return 0;
}

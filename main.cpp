
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

using namespace std;

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

    // 3. Generate recommendations
    RecommendationEngine engine;

    vector<Recommendation> recommendations =
        engine.recommendCareers(user, careers);

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

    // 4. Let the user select a recommended career
    int choice;

    cout << "\nEnter the number of the career to explore: ";

    if (!(cin >> choice) ||
        choice < 1 ||
        choice > static_cast<int>(recommendations.size()))
    {
        cout << "Invalid career selection.\n";
        return 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    const Career& selectedCareer =
        recommendations[choice - 1].career;

    cout << "\nSelected career: "
         << selectedCareer.getName() << "\n";

    cout << "Education requirement: "
         << selectedCareer.getRequiredEducation() << "\n";

    // 5. Load and display the selected career's roadmap
    Roadmap roadmap(selectedCareer.getName());

    if (!roadmap.loadStepsFromCSV("roadmap_data.csv"))
    {
        cout << "\nNo roadmap found for "
             << selectedCareer.getName() << ".\n";
        cout << "Check that the career name matches the CSV exactly.\n";
        return 0;
    }

    cout << "\n========== YOUR CAREER ROADMAP ==========\n";

    for (const Step& step : roadmap.getSteps())
    {
        cout << "\nStep " << step.getSnum()
             << ": " << step.getTitle() << "\n";

        cout << step.getDescription() << "\n";
    }

    cout << "\n=========================================\n";
    cout << "Thank you for using Career Navigator!\n";

    return 0;
}

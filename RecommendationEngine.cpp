#include "RecommendationEngine.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Convert text to lowercase and remove leading/trailing spaces
string normalize(string text)
{
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char ch) {
            return static_cast<char>(tolower(ch));
        });

    size_t start = text.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = text.find_last_not_of(" \t\r\n");

    return text.substr(start, end - start + 1);
}

// Split comma-separated user input into individual values
vector<string> splitInput(const string& input)
{
    vector<string> values;
    string item;
    stringstream ss(input);

    while (getline(ss, item, ','))
    {
        item = normalize(item);

        if (!item.empty() && item != "not set")
            values.push_back(item);
    }

    return values;
}

// Calculate match score for one career
double RecommendationEngine::calculateScore(
    const User& user,
    const Career& career)
{
    // 1. Education match: 40%
    double educationScore = 0.0;

    string userEducation = normalize(user.getEducation());
    string requiredEducation =
        normalize(career.getRequiredEducation());

    if (!userEducation.empty() &&
        userEducation != "not set" &&
        userEducation == requiredEducation)
    {
        educationScore = 1.0;
    }

    // 2. Interest match: 30%
    vector<string> userInterests =
        splitInput(user.getInterests());

    const auto& careerInterests = career.getInterests();

    int matchingInterests = 0;

    for (const auto& careerInterest : careerInterests)
    {
        string requiredInterest = normalize(careerInterest);

        for (const auto& userInterest : userInterests)
        {
            if (requiredInterest == userInterest)
            {
                matchingInterests++;
                break;
            }
        }
    }

    double interestScore = 0.0;

    if (!careerInterests.empty())
    {
        interestScore =
            static_cast<double>(matchingInterests) /
            careerInterests.size();
    }

    // 3. Skill match: 30%
    vector<string> userSkills =
        splitInput(user.getSkills());

    const auto& requiredSkills = career.getRequiredSkills();

    int matchingSkills = 0;

    for (const auto& requiredSkill : requiredSkills)
    {
        string normalizedSkill = normalize(requiredSkill);

        if (find(userSkills.begin(), userSkills.end(),
                 normalizedSkill) != userSkills.end())
        {
            matchingSkills++;
        }
    }

    double skillScore = 0.0;

    if (!requiredSkills.empty())
    {
        skillScore =
            static_cast<double>(matchingSkills) /
            requiredSkills.size();
    }

    // 4. Calculate weighted final score
    return (0.40 * educationScore) +
           (0.30 * interestScore) +
           (0.30 * skillScore);
}

// Recommend relevant careers from highest to lowest score
vector<Recommendation> RecommendationEngine::recommendCareers(
    const User& user,
    const vector<Career>& careers)
{
    vector<Recommendation> recommendations;

    // Change this value to adjust how selective recommendations are.
    const double MINIMUM_SCORE = 0.50;

    for (const auto& career : careers)
    {
        double score = calculateScore(user, career);

        // Include only careers that meet the minimum score.
        if (score >= MINIMUM_SCORE)
        {
            recommendations.push_back({career, score});
        }
    }

    // Sort from highest score to lowest
    sort(recommendations.begin(), recommendations.end(),
        [](const Recommendation& a, const Recommendation& b)
        {
            return a.score > b.score;
        });

    return recommendations;
}


#include "RecommendationEngine.h"
#include <algorithm>

// Calculates how well one career matches the user
double RecommendationEngine::calculateScore(
    const User& user,
    const Career& career
) {
    // 1. Education score
    double educationScore;

    if (user.getEducation() == career.getRequiredEducation()) {
        educationScore = 1.0;
    }
    else {
        educationScore = 0.0;
    }

    // 2. Interest score
    double interestScore = 0.0;

    for (const auto& userInterest : user.getInterests()) {
        for (const auto& careerInterest : career.getInterests()) {
            if (userInterest == careerInterest) {
                interestScore = 1.0;
                break;
            }
        }

        if (interestScore == 1.0) {
            break;
        }
    }

    // 3. Skill score
    int matchingSkills = 0;
    const auto& userSkills = user.getSkills();
    const auto& requiredSkills = career.getRequiredSkills();

    for (const auto& requiredSkill : requiredSkills) {
        for (const auto& userSkill : userSkills) {
            if (requiredSkill == userSkill) {
                matchingSkills++;
                break;
            }
        }
    }

    double skillScore = 0.0;

    if (!requiredSkills.empty()) {
        skillScore = static_cast<double>(matchingSkills)
                   / requiredSkills.size();
    }

    // 4. Weighted final score
    double finalScore =
        (0.40 * educationScore) +
        (0.30 * interestScore) +
        (0.30 * skillScore);

    return finalScore;
}

// Calculates scores for all careers and ranks them
std::vector<Recommendation> RecommendationEngine::recommendCareers(
    const User& user,
    const std::vector<Career>& careers
) {
    std::vector<Recommendation> recommendations;

    // Calculate one score for each career
    for (const auto& career : careers) {
        double score = calculateScore(user, career);

        Recommendation result;
        result.career = career;
        result.score = score;

        recommendations.push_back(result);
    }

    // Sort from highest score to lowest score
    std::sort(
        recommendations.begin(),
        recommendations.end(),
        [](const Recommendation& a, const Recommendation& b) {
            return a.score > b.score;
        }
    );

    return recommendations;
}

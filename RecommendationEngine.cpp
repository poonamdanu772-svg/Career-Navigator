
#include "RecommendationEngine.h"
#include <algorithm>

// Calculate the match score for one career
double RecommendationEngine::calculateScore(
    const User& user,
    const Career& career
) {
    // 1. Education score
    double educationScore = 0.0;

    if (user.getEducation() == career.getRequiredEducation()) {
        educationScore = 1.0;
    }

    // 2. Interest score
    int matchingInterests = 0;

    const auto& userInterests = user.getInterests();
    const auto& careerInterests = career.getInterests();

    for (const auto& careerInterest : careerInterests) {
        for (const auto& userInterest : userInterests) {
            if (careerInterest == userInterest) {
                matchingInterests++;
                break;
            }
        }
    }

    double interestScore = 0.0;

    if (!careerInterests.empty()) {
        interestScore =
            static_cast<double>(matchingInterests)
            / careerInterests.size();
    }


   
    // 3. Skill score
    int matchingSkills = 0;

    const auto& userSkills = user.getSkills();
    const auto& requiredSkills = career.getRequiredSkills();

    for (const auto& requiredSkill : requiredSkills) {
        if (std::find(
                userSkills.begin(),
                userSkills.end(),
                requiredSkill
            ) != userSkills.end()) {
            matchingSkills++;
        }
    }

    double skillScore = 0.0;

    if (!requiredSkills.empty()) {
        skillScore =
            static_cast<double>(matchingSkills)
            / requiredSkills.size();
    }

    // 4. Calculate the weighted final score
    
    double finalScore =
    (0.40 * educationScore) +
    (0.30 * interestScore) +
    (0.30 * skillScore);

    // Keep the score between 0.0 and 1.0
    if (finalScore < 0.0) {
        finalScore = 0.0;
    }

    if (finalScore > 1.0) {
        finalScore = 1.0;
    }
    return finalScore;

}

// Recommend careers from highest score to lowest
std::vector<Recommendation> RecommendationEngine::recommendCareers(
    const User& user,
    const std::vector<Career>& careers
) {
    std::vector<Recommendation> recommendations;

    for (const auto& career : careers) {
        double score = calculateScore(user, career);
        Recommendation result {career, score};

    

        recommendations.push_back(result);
    }

    std::sort(
        recommendations.begin(),
        recommendations.end(),
        [](const Recommendation& a, const Recommendation& b) {
            return a.score > b.score;
        }
    );

    return recommendations;
}

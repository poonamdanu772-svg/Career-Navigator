
#ifndef RECOMMENDATION_ENGINE_H
#define RECOMMENDATION_ENGINE_H

#include <vector>
#include "User.h"
#include "Career.h"

using namespace std;

// Stores one career and its matching score
struct Recommendation {
    Career career;
    double score;
};

class RecommendationEngine {
public:
    // Calculates the match score for one career
    double calculateScore(const User& user, const Career& career);

    // Evaluates and ranks a list of careers
    vector<Recommendation> recommendCareers(
        const User& user,
        const vector<Career>& careers
    );
};

#endif

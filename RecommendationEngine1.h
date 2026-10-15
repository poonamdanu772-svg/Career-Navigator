
#ifndef RECOMMENDATION_ENGINE_H
#define RECOMMENDATION_ENGINE_H

#include <vector>
#include "User.h"
#include "Career.h"

struct Recommendation {
    Career career;
    double score;
};

class RecommendationEngine {
public:
    double calculateScore(const User& user, const Career& career);

    std::vector<Recommendation> recommendCareers(
        const User& user,
        const std::vector<Career>& careers
    );
};

#endif

#ifndef RECOMMENDATION_ENGINE_H
#define RECOMMENDATION_ENGINE_H

#include <string>
#include <vector>
#include "User.h"
#include "Career.h"

struct Recommendation {
    Career career;
    double score; // Score is between 0.0 and 1.0.
};

// ABSTRACTION + POLYMORPHISM.
// This abstract base class describes what every scoring strategy must do.
class RecommendationStrategy {
public:
    virtual double calculateScore(const User& user, const Career& career) const = 0;

    //  VIRTUAL DESTRUCTOR makes deleting derived objects through a base pointer safe.
    virtual ~RecommendationStrategy() {}
};

// INHERITANCE - this strategy IS-A RecommendationStrategy.
class BalancedRecommendationStrategy : public RecommendationStrategy {
public:
    double calculateScore(const User& user, const Career& career) const override;
};

// INHERITANCE - another strategy IS-A RecommendationStrategy.
class InterestFocusedStrategy : public RecommendationStrategy {
public:
    double calculateScore(const User& user, const Career& career) const override;
};

class RecommendationEngine {
public:
    // ABSTRACTION - engine uses the base-class interface, not strategy internals.
    std::vector<Recommendation> recommendCareers(
        const User& user,
        const std::vector<Career>& careers,
        const RecommendationStrategy& strategy
    ) const;
};

#endif

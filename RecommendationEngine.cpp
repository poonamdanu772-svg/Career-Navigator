#include "RecommendationEngine.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace {
    std::string normalize(std::string text) {
        for (char& ch : text) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        std::size_t first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        std::size_t last = text.find_last_not_of(" \t\r\n");
        return text.substr(first, last - first + 1);
    }

    std::vector<std::string> splitInput(const std::string& input) {
        std::vector<std::string> values;
        std::stringstream stream(input);
        std::string item;
        while (std::getline(stream, item, ',')) {
            item = normalize(item);
            if (!item.empty() && item != "not set") values.push_back(item);
        }
        return values;
    }

    bool contains(const std::vector<std::string>& values, const std::string& target) {
        for (const std::string& value : values) {
            if (value == target) return true;
        }
        return false;
    }

    double educationMatch(const User& user, const Career& career) {
        std::string userEducation = normalize(user.getEducation());
        std::string requiredEducation = normalize(career.getRequiredEducation());

        if (userEducation.empty() || userEducation == "not set" ||
            requiredEducation.empty()) return 0.0;

        std::vector<std::string> userWords;
        std::stringstream userStream(userEducation);
        std::string word;
        while (userStream >> word) {
            if (word.size() > 2 && word != "and" && word != "for" &&
                word != "the" && word != "with" && word != "related") {
                userWords.push_back(word);
            }
        }

        for (const std::string& userWord : userWords) {
            if (requiredEducation.find(userWord) != std::string::npos) return 1.0;
        }

        // Common shorthand for computing degrees.
        if ((userEducation.find("cse") != std::string::npos ||
             userEducation.find("computer") != std::string::npos ||
             userEducation.find("it") == 0) &&
            (requiredEducation.find("computer") != std::string::npos ||
             requiredEducation.find("information technology") != std::string::npos ||
             requiredEducation.find("related") != std::string::npos)) return 1.0;

        return 0.0;
    }

    double interestMatch(const User& user, const Career& career) {
        std::vector<std::string> interests = splitInput(user.getInterests());
        const std::vector<std::string>& required = career.getInterests();
        if (required.empty()) return 0.0;

        int matches = 0;
        for (const std::string& item : required) {
            if (contains(interests, normalize(item))) ++matches;
        }
        return static_cast<double>(matches) / required.size();
    }

    double skillMatch(const User& user, const Career& career) {
        std::vector<std::string> skills = splitInput(user.getSkills());
        const std::vector<std::string>& required = career.getRequiredSkills();
        if (required.empty()) return 0.0;

        int matches = 0;
        for (const std::string& item : required) {
            if (contains(skills, normalize(item))) ++matches;
        }
        return static_cast<double>(matches) / required.size();
    }

    double weightedScore(const User& user, const Career& career,
                         double educationWeight, double interestWeight,
                         double skillWeight) {
        return educationWeight * educationMatch(user, career) +
               interestWeight * interestMatch(user, career) +
               skillWeight * skillMatch(user, career);
    }
}

// Balanced: education 20%, interests 40%, skills 40%.
double BalancedRecommendationStrategy::calculateScore(
    const User& user, const Career& career) const {
    return weightedScore(user, career, 0.20, 0.40, 0.40);
}

// Interest-focused: education 15%, interests 60%, skills 25%.
double InterestFocusedStrategy::calculateScore(
    const User& user, const Career& career) const {
    return weightedScore(user, career, 0.15, 0.60, 0.25);
}

std::vector<Recommendation> RecommendationEngine::recommendCareers(
    const User& user, const std::vector<Career>& careers,
    const RecommendationStrategy& strategy) const {
    std::vector<Recommendation> recommendations;

    for (const Career& career : careers) {
        // RUNTIME POLYMORPHISM.
        // This virtual call uses the selected derived strategy's implementation.
        double score = strategy.calculateScore(user, career);
        if (score > 0.0) {
            Recommendation result = {career, score};
            recommendations.push_back(result);
        }
    }

    // Higher scores first; if scores tie, keep the original CSV order.
    std::stable_sort(recommendations.begin(), recommendations.end(),
        [](const Recommendation& a, const Recommendation& b) {
            return a.score > b.score;
        });

    return recommendations;
}

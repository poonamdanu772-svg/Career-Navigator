#include "RecommendationEngine.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <utility>

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
            if (!item.empty() && item != "not set") {
                values.push_back(item);
            }
        }
        return values;
    }

    // Convert everyday phrases into career-related terms used by the CSV.
    // The original phrase is kept too, so exact CSV tags still work.
    std::vector<std::string> expandInput(const std::string& input) {
        std::vector<std::string> expanded = splitInput(input);

        const std::vector<std::pair<std::string, std::vector<std::string>>> aliases = {
            {"sharp mind", {"sharp thinking", "analytical thinking", "critical thinking",
                            "problem solving", "analysis", "solving puzzles"}},
            {"sharp brain", {"sharp thinking", "analytical thinking", "critical thinking",
                             "problem solving", "analysis"}},
            {"intelligent", {"analytical thinking", "critical thinking", "problem solving"}},
            {"smart", {"analytical thinking", "critical thinking", "problem solving"}},
            {"quick thinker", {"quick thinking", "decision making", "clinical reasoning"}},
            {"quick thinking", {"decision making", "clinical reasoning", "problem solving"}},
            {"good listener", {"active listening", "listening", "empathy", "communication"}},
            {"listener", {"active listening", "listening", "empathy", "communication"}},
            {"listening well", {"active listening", "listening", "empathy"}},
            {"talking to friends", {"talking to people", "people", "communication", "relationships"}},
            {"talking to people", {"people", "communication", "relationships"}},
            {"chatting", {"communication", "people", "relationships"}},
            {"chatty", {"communication", "people", "relationships"}},
            {"making friends", {"relationships", "people", "communication", "teamwork"}},
            {"socializing", {"relationships", "people", "communication", "community"}},
            {"helping friends", {"helping others", "helping people", "caregiving", "service"}},
            {"helping people", {"helping others", "service", "caregiving", "community"}},
            {"kind", {"empathy", "helping others", "caregiving"}},
            {"caring", {"caregiving", "helping others", "empathy"}},
            {"patient", {"patience", "caregiving", "patient care"}},
            {"good memory", {"concentration", "accuracy", "attention to detail"}},
            {"observant", {"observation", "attention to detail", "investigation"}},
            {"noticing details", {"attention to detail", "observation", "accuracy"}},
            {"curious", {"curiosity", "research", "investigation", "asking questions"}},
            {"asking questions", {"research", "investigation", "communication"}},
            {"solving puzzles", {"problem solving", "logical thinking", "critical thinking"}},
            {"fixing things", {"troubleshooting", "maintenance", "repair procedures", "mechanics"}},
            {"building things", {"building things", "building systems", "making things"}},
            {"drawing", {"art", "design", "visual communication", "illustration", "sketching"}},
            {"creative", {"creativity", "design", "art", "visual communication"}},
            {"good at art", {"art", "creativity", "design", "visual communication"}},
            {"gaming", {"technology", "competition", "testing", "building products"}},
            {"video games", {"technology", "competition", "testing", "building products"}},
            {"skating", {"sports", "fitness", "movement", "sport skills"}},
            {"playing sports", {"sports", "fitness", "movement", "teamwork"}},
            {"watching videos", {"media", "film", "visual storytelling", "visual effects"}},
            {"making videos", {"media", "film", "visual storytelling", "editing"}},
            {"taking photos", {"photography", "visual communication", "creativity"}},
            {"cooking", {"food", "food production", "cooking techniques", "making things"}},
            {"baking", {"food", "food production", "baking techniques", "making things"}},
            {"travelling", {"travel", "cultures", "planning", "communication"}},
            {"traveling", {"travel", "cultures", "planning", "communication"}},
            {"organizing things", {"organization", "planning", "operations", "event planning"}},
            {"explaining things", {"teaching", "communication", "public speaking", "lesson planning"}},
            {"good at explaining", {"teaching", "communication", "public speaking", "lesson planning"}},
            {"leading people", {"team leadership", "leadership", "teamwork", "planning"}},
            {"leader", {"team leadership", "leadership", "teamwork", "planning"}},
            {"good with numbers", {"numbers", "mathematics", "statistics", "data", "accounting"}},
            {"maths", {"mathematics", "numbers", "statistics"}},
            {"nature lover", {"nature", "environment", "living things", "sustainability"}},
            {"animals", {"animals", "animal care", "animal welfare", "biology"}},
            {"love animals", {"animals", "animal care", "animal welfare", "biology"}},
            {"good at computers", {"technology", "programming", "digital literacy", "building products"}},
            {"computers", {"technology", "programming", "digital literacy"}},
            {"writing stories", {"storytelling", "writing", "language", "creativity"}},
            {"arguing", {"argumentation", "debate", "persuasion", "critical thinking"}},
            {"persuasive", {"persuasion", "communication", "negotiation", "sales"}},
            {"good at teamwork", {"teamwork", "collaboration", "communication"}},
            {"team player", {"teamwork", "collaboration", "communication"}},
            {"quick learner", {"learning", "adaptation strategies", "practice"}},
            {"hardworking", {"discipline", "time management", "goal setting"}},
            {"focused", {"concentration", "attention to detail", "accuracy"}},
            {"good at observing", {"observation", "attention to detail", "investigation"}}
        };

        for (const std::string& value : splitInput(input)) {
            for (const auto& alias : aliases) {
                if (value == alias.first) {
                    for (const std::string& mapped : alias.second) {
                        expanded.push_back(normalize(mapped));
                    }
                }
            }
        }

        // Remove duplicate tags after alias expansion.
        std::vector<std::string> uniqueValues;
        for (const std::string& value : expanded) {
            if (std::find(uniqueValues.begin(), uniqueValues.end(), value) == uniqueValues.end()) {
                uniqueValues.push_back(value);
            }
        }
        return uniqueValues;
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
            requiredEducation.empty()) {
            return 0.0;
        }

        if (userEducation == requiredEducation) return 1.0;

        if (requiredEducation.find(userEducation) != std::string::npos) {
            return 1.0;
        }

        return 0.0;
    }

    double interestMatch(const User& user, const Career& career) {
        std::vector<std::string> interests = expandInput(user.getInterests());
        const std::vector<std::string>& required = career.getInterests();
        if (required.empty()) return 0.0;

        int matches = 0;
        for (const std::string& item : required) {
            if (contains(interests, normalize(item))) ++matches;
        }
        return static_cast<double>(matches) / required.size();
    }

    double skillMatch(const User& user, const Career& career) {
        std::vector<std::string> skills = expandInput(user.getSkills());
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
        // Runtime polymorphism: calls the selected strategy implementation.
        double score = strategy.calculateScore(user, career);
        if (score > 0.0) {
            Recommendation result = {career, score};
            recommendations.push_back(result);
        }
    }

    // Higher scores first; ties retain the original CSV order.
    std::stable_sort(recommendations.begin(), recommendations.end(),
        [](const Recommendation& a, const Recommendation& b) {
            return a.score > b.score;
        });

    return recommendations;
}

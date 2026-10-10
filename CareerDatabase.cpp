#include "CareerDatabase.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

static vector<string> splitValues(const string& text) {
    vector<string> values;
    stringstream stream(text);
    string value;

    while (getline(stream, value, '|')) {
        values.push_back(value);
    }

    return values;
}

bool CareerDatabase::loadFromFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << '\n';
        return false;
    }

    string line;

    // Skip the header row.
    if (!getline(file, line)) {
        cerr << "Error: CSV file is empty.\n";
        return false;
    }

    vector<Career> loadedCareers;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        string name;
        string education;
        string interestsText;
        string skillsText;

        stringstream row(line);

        if (!getline(row, name, ',') ||
            !getline(row, education, ',') ||
            !getline(row, interestsText, ',') ||
            !getline(row, skillsText)) {
            cerr << "Skipping invalid row: " << line << '\n';
            continue;
        }

        loadedCareers.emplace_back(
            name,
            education,
            splitValues(interestsText),
            splitValues(skillsText)
        );
    }

    if (file.bad()) {
        cerr << "Error while reading " << filename << '\n';
        return false;
    }

    careers = std::move(loadedCareers);

    cout << "Loaded " << careers.size()
         << " careers successfully.\n";

    return true;
}

void CareerDatabase::addCareer(const Career& career) {
    careers.push_back(career);
}

const vector<Career>& CareerDatabase::getAllCareers() const {
    return careers;
}
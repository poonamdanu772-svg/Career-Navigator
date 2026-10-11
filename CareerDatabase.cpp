
#include "CareerDatabase.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

// Split interests or skills separated by |
static vector<string> splitValues(const string& text)
{
    vector<string> values;
    stringstream stream(text);
    string value;

    while (getline(stream, value, '|'))
    {
        // Remove leading spaces
        size_t start = value.find_first_not_of(" \t\r\n");

        if (start == string::npos)
            continue;

        // Remove trailing spaces
        size_t end = value.find_last_not_of(" \t\r\n");
        value = value.substr(start, end - start + 1);

        values.push_back(value);
    }

    return values;
}

// Parse one CSV row, respecting quoted commas and escaped quotes
static vector<string> parseCSVLine(const string& line)
{
    vector<string> fields;
    string field;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); i++)
    {
        char ch = line[i];

        if (ch == '"')
        {
            if (insideQuotes && i + 1 < line.size()
                && line[i + 1] == '"')
            {
                field += '"';
                i++;
            }
            else
            {
                insideQuotes = !insideQuotes;
            }
        }
        else if (ch == ',' && !insideQuotes)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += ch;
        }
    }

    fields.push_back(field);
    return fields;
}

bool CareerDatabase::loadFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open " << filename << '\n';
        return false;
    }

    string line;

    // Skip the header row
    if (!getline(file, line))
    {
        cerr << "Error: CSV file is empty.\n";
        return false;
    }

    vector<Career> loadedCareers;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);

        // Expected columns:
        // name, career_category, education, interests, skills
        if (fields.size() != 5)
        {
            cerr << "Skipping invalid row: " << line << '\n';
            continue;
        }

        string name = fields[0];
        string education = fields[2];
        string interestsText = fields[3];
        string skillsText = fields[4];

        loadedCareers.emplace_back(
            name,
            education,
            splitValues(interestsText),
            splitValues(skillsText)
        );
    }

    if (file.bad())
    {
        cerr << "Error while reading " << filename << '\n';
        return false;
    }

    if (loadedCareers.empty())
    {
        cerr << "Error: No valid career records were loaded.\n";
        return false;
    }

    careers = std::move(loadedCareers);

    cout << "Loaded " << careers.size()
         << " careers successfully.\n";

    return true;
}

void CareerDatabase::addCareer(const Career& career)
{
    careers.push_back(career);
}

const vector<Career>& CareerDatabase::getAllCareers() const
{
    return careers;
}

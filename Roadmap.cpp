#include "Step.h"
#include "Roadmap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

namespace
{
    string trim(const string& text)
    {
        size_t start = text.find_first_not_of(" \t\r\n");

        if (start == string::npos)
            return "";

        size_t end = text.find_last_not_of(" \t\r\n");
        return text.substr(start, end - start + 1);
    }

    string normalize(string text)
    {
        text = trim(text);

        for (char& ch : text)
            ch = static_cast<char>(
                tolower(static_cast<unsigned char>(ch)));

        return text;
    }

    vector<string> parseCSVLine(const string& line)
    {
        vector<string> fields;
        string field;
        bool insideQuotes = false;

        for (size_t i = 0; i < line.size(); i++)
        {
            char ch = line[i];

            if (ch == '"')
            {
                if (insideQuotes &&
                    i + 1 < line.size() &&
                    line[i + 1] == '"')
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
                fields.push_back(trim(field));
                field.clear();
            }
            else
            {
                field += ch;
            }
        }

        fields.push_back(trim(field));
        return fields;
    }
}

Roadmap::Roadmap(const string& careerName, const string& pathType)
    : careerName(careerName), pathType(pathType)
{
}

string Roadmap::getCareerName() const
{
    return careerName;
}

string Roadmap::getPathType() const
{
    return pathType;
}

void Roadmap::addStep(const Step& step)
{
    steps.push_back(step);
}

const vector<Step>& Roadmap::getSteps() const
{
    return steps;
}

bool Roadmap::loadStepsFromCSV(const string& filename)
{
    steps.clear();

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Could not open " << filename << ".\n";
        return false;
    }

    string line;

    // Skip the header row.
    if (!getline(file, line))
        return false;

    while (getline(file, line))
    {
        if (trim(line).empty())
            continue;

        vector<string> fields = parseCSVLine(line);

        // career_name, path_type, step_number, title, description
        if (fields.size() != 5)
        {
            cerr << "Skipping invalid roadmap row: "
                 << line << '\n';
            continue;
        }

        if (normalize(fields[0]) != normalize(careerName) ||
            normalize(fields[1]) != normalize(pathType))
        {
            continue;
        }

        try
        {
            int stepNumber = stoi(fields[2]);

            steps.emplace_back(
                stepNumber,
                fields[3],
                fields[4]
            );
        }
        catch (const exception&)
        {
            cerr << "Skipping row with invalid step number: "
                 << line << '\n';
        }
    }

    return !steps.empty();
}

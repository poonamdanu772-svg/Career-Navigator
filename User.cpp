// User.cpp
// Career Navigator - Module 1: User Profile Module
// Definitions of the User class member functions.

#include "User.h"
#include <iostream>
#include <stdexcept>

// Helper (visible only in this file): true if string is empty or only whitespace
namespace {
    bool isBlank(const std::string& text) {
        return text.find_first_not_of(" \t\n\r") == std::string::npos;
    }
}

// ---------- Constructors ----------

// Default constructor: sets safe, sensible default values
User::User()
    : name("Not Set"), age(18), education("Not Set"), stream("Not Set"),
      interests("Not Set"), skills("Not Set"), budget(0.0),
      careerGoal("Not Set") {}

// Parameterized constructor: validates using the setters.
// Throws std::invalid_argument if a required value is invalid.
User::User(const std::string& name, int age, const std::string& education,
           const std::string& stream, const std::string& interests,
           const std::string& skills, double budget,
           const std::string& careerGoal)
    : name("Not Set"), age(18), education("Not Set"), stream("Not Set"),
      interests("Not Set"), skills("Not Set"), budget(0.0),
      careerGoal("Not Set")
{
    if (!setName(name))
        throw std::invalid_argument("Name must not be empty.");
    if (!setAge(age))
        throw std::invalid_argument("Age must be between 1 and 120.");
    if (!setBudget(budget))
        throw std::invalid_argument("Budget must not be negative.");
    if (!setCareerGoal(careerGoal))
        throw std::invalid_argument("Career goal must not be empty.");

    // These fields are optional, so their setters always accept them
    setEducation(education);
    setStream(stream);
    setInterests(interests);
    setSkills(skills);
}

// ---------- Getters ----------
std::string User::getName() const       { return name; }
int         User::getAge() const        { return age; }
std::string User::getEducation() const  { return education; }
std::string User::getStream() const     { return stream; }
std::string User::getInterests() const  { return interests; }
std::string User::getSkills() const     { return skills; }
double      User::getBudget() const     { return budget; }
std::string User::getCareerGoal() const { return careerGoal; }

// ---------- Setters with validation ----------

// Name is required
bool User::setName(const std::string& newName) {
    if (isBlank(newName)) return false;
    name = newName;
    return true;
}

// Age must be between MIN_AGE and MAX_AGE
bool User::setAge(int newAge) {
    if (newAge < MIN_AGE || newAge > MAX_AGE) return false;
    age = newAge;
    return true;
}

// Education, stream, interests, skills are optional: blank becomes "Not Set"
bool User::setEducation(const std::string& newEducation) {
    education = isBlank(newEducation) ? "Not Set" : newEducation;
    return true;
}

bool User::setStream(const std::string& newStream) {
    stream = isBlank(newStream) ? "Not Set" : newStream;
    return true;
}

bool User::setInterests(const std::string& newInterests) {
    interests = isBlank(newInterests) ? "Not Set" : newInterests;
    return true;
}

bool User::setSkills(const std::string& newSkills) {
    skills = isBlank(newSkills) ? "Not Set" : newSkills;
    return true;
}

// Budget must not be negative
bool User::setBudget(double newBudget) {
    if (newBudget < 0.0) return false;
    budget = newBudget;
    return true;
}

// Career goal is required
bool User::setCareerGoal(const std::string& newCareerGoal) {
    if (isBlank(newCareerGoal)) return false;
    careerGoal = newCareerGoal;
    return true;
}

// ---------- Display ----------
void User::displayProfile() const {
    std::cout << "\n========== USER PROFILE ==========\n";
    std::cout << "Name        : " << name << "\n";
    std::cout << "Age         : " << age << "\n";
    std::cout << "Education   : " << education << "\n";
    std::cout << "Stream      : " << stream << "\n";
    std::cout << "Interests   : " << interests << "\n";
    std::cout << "Skills      : " << skills << "\n";
    std::cout << "Budget      : " << budget << "\n";
    std::cout << "Career Goal : " << careerGoal << "\n";
    std::cout << "==================================\n";
}

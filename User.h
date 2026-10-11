// User.h
// Career Navigator - Module 1: User Profile Module
// Declaration of the User class.

#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    // ---------- Private data members (Encapsulation) ----------
    std::string name;        // Full name
    int         age;         // Age in years
    std::string education;   // Current education level
    std::string stream;      // Academic stream / specialization
    std::string interests;   // Career interests
    std::string skills;      // Existing skills
    double      budget;      // Amount available for education/training
    std::string careerGoal;  // Desired career goal

public:
    // Limits used for age validation
    static const int MIN_AGE = 1;
    static const int MAX_AGE = 120;

    // ---------- Constructors ----------
    User();   // Default constructor
    User(const std::string& name, int age, const std::string& education,
         const std::string& stream, const std::string& interests,
         const std::string& skills, double budget,
         const std::string& careerGoal);   // Parameterized constructor

    // ---------- Getters (const: they do not modify the object) ----------
    std::string getName() const;
    int         getAge() const;
    std::string getEducation() const;
    std::string getStream() const;
    std::string getInterests() const;
    std::string getSkills() const;
    double      getBudget() const;
    std::string getCareerGoal() const;

    // ---------- Setters (return true if accepted, false if rejected) ----------
    bool setName(const std::string& newName);
    bool setAge(int newAge);
    bool setEducation(const std::string& newEducation);
    bool setStream(const std::string& newStream);
    bool setInterests(const std::string& newInterests);
    bool setSkills(const std::string& newSkills);
    bool setBudget(double newBudget);
    bool setCareerGoal(const std::string& newCareerGoal);

    // ---------- Other functions ----------
    void displayProfile() const;   // Prints the full profile
};

#endif // USER_H

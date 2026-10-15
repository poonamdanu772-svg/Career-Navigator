# Career Navigator 🚀

**Career Navigator** is a C++-based career guidance application that helps users explore career opportunities based on their education, interests, skills, and career goals. It recommends suitable careers and provides degree-based and skill-based preparation roadmaps to help users plan their next steps.

## 🎯 Project Objectives

- Help users discover careers that align with their interests and skills.
- Provide personalized career recommendations using a weighted scoring system.
- Display career descriptions and match percentages.
- Offer degree-based and skill-based preparation roadmaps.
- Demonstrate object-oriented programming concepts through a modular C++ application.

## ✨ Features

- **User Profile Management:** Collects information such as name, age, education, academic stream, interests, skills, training budget, and long-term career goals.
- **Personalized Recommendations:** Ranks careers according to how well they match the user's profile.
- **Multiple Recommendation Strategies:**
  - Balanced: Education 20%, Interests 40%, Skills 40%.
  - Interest-focused: Education 15%, Interests 60%, Skills 25%.
- **Career Descriptions:** Displays a short description and match percentage for recommended careers.
- **Top Five Matches:** Shows the five highest-ranked unique career recommendations.
- **Dual Preparation Roadmaps:** Provides degree-based and skill-based pathways for supported careers.
- **Interactive Exploration:** Allows users to explore recommended careers and switch between preparation paths.
- **CSV-Based Data Management:** Loads career information and roadmap steps from CSV files.

## 🛠️ Technologies Used

- **Programming Language:** C++
- **Standard:** C++17
- **Compiler:** GCC / G++
- **Data Storage:** CSV files
- **Development Environment:** Visual Studio Code
- **Version Control:** Git and GitHub

## 🧠 Object-Oriented Programming Concepts

Career Navigator demonstrates the following OOP concepts:

- **Encapsulation:** Classes such as `User`, `Career`, `Step`, and `Roadmap` manage their data through private members and public methods.
- **Inheritance:** Degree-based and skill-based career paths derive from the common `CareerPath` base class.
- **Abstraction:** The abstract `CareerPath` class defines a common interface for displaying preparation paths.
- **Runtime Polymorphism:** Virtual functions allow the program to display the selected career path through a base-class pointer.
- **Strategy Pattern:** Different recommendation strategies implement a common interface, allowing the recommendation engine to use different scoring approaches.

## 📁 Project Structure

```text
Career-Navigator/
│
├── main.cpp
├── User.h
├── User.cpp
├── Career.h
├── Career.cpp
├── CareerDatabase.h
├── CareerDatabase.cpp
├── RecommendationEngine.h
├── RecommendationEngine.cpp
├── CareerPath.h
├── DegreeCareerPath.h
├── SkillBasedCareerPath.h
├── Roadmap.h
├── Roadmap.cpp
├── Step.h
├── Step.cpp
├── careers.csv
├── roadmap_data.csv
├── .gitignore
└── README.md
```

*The structure above describes the expected core files. Your actual repository may contain additional files.*

## ⚙️ Prerequisites

Before running the project, ensure you have:

- A C++ compiler supporting C++17, such as GCC/G++.
- Visual Studio Code or another C++ development environment.
- Git, if you want to clone or contribute to the repository.

## 🚀 Installation and Execution

### 1. Clone the repository

```bash
git clone https://github.com/poonamdanu772-svg/Career-Navigator.git
```

### 2. Open the project folder

```bash
cd Career-Navigator
```

If your repository contains a nested project folder, navigate into the directory containing `main.cpp` and the source files.

### 3. Compile the project

Run the following command from the directory containing the source files:

```bash
g++ -std=c++17 main.cpp User.cpp Career.cpp CareerDatabase.cpp RecommendationEngine.cpp Step.cpp Roadmap.cpp -o CareerNavigator.exe
```

### 4. Run the application

On Windows PowerShell:

```powershell
.\CareerNavigator.exe
```

On Linux or macOS, use an appropriate executable name and run it with `./`.

## 📄 Data Files

### `careers.csv`

Stores the career database, including career names, descriptions, education requirements, interests, and skills.

The expected column order is:

```text
name,description,education,interests,skills
```

Interests and skills are separated by the `|` character. Descriptions containing commas should be enclosed in double quotes.

### `roadmap_data.csv`

Stores the preparation steps associated with different careers and pathway types.

The expected column order is:

```text
career_name,path_type,step_number,title,description
```

The pathway type identifies whether a step belongs to a degree-based or skill-based roadmap.

**Important:** Keep both CSV files in the working directory from which the program is launched. Their column structure must match the CSV parsers implemented in the source code.

## 🔄 How It Works

1. The user enters their profile information.
2. The application loads the career database from `careers.csv`.
3. The user selects a recommendation strategy.
4. The recommendation engine calculates scores and ranks matching careers.
5. The application displays the top five careers, their match percentages, and descriptions.
6. The user selects a recommended career.
7. The user explores its degree-based or skill-based preparation roadmap.
8. The user can switch pathways, explore another recommended career, or exit.

## 🔮 Future Enhancements

Potential future improvements include:

- Adding more detailed career information, including salary ranges and certifications.
- Improving recommendation accuracy with additional profile factors.
- Introducing a graphical user interface.
- Adding a database-backed career repository.
- Supporting more career pathways and personalized progress tracking.

## 👥 Contributors

Developed as a team academic project to demonstrate C++ programming, object-oriented design, recommendation logic, and roadmap generation.

## 📜 License

No license has been specified yet. Add a license if you intend to permit others to use, modify, or distribute the project.

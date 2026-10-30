#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QScrollArea>
#include <QFrame>

#include "User.h"
#include "Career.h"
#include "CareerDatabase.h"
#include "RecommendationEngine.h"
#include "CareerPath.h"
#include "DegreeCareerPath.h"
#include "SkillBasedCareerPath.h"
#include "Roadmap.h"
#include "Step.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Navigation
    void onNavButtonClicked();
    void toggleTheme();

    // Profile page slots
    void onApplyPresetSoftware();
    void onApplyPresetDataScience();
    void onApplyPresetDesign();
    void onApplyPresetHealthcare();
    void onApplyPresetBusiness();
    void onInterestTagClicked();
    void onSkillTagClicked();
    void onSaveProfileAndMatch();

    // Strategy slots
    void onStrategyChanged();

    // Recommendation slots
    void onExploreCareerRoadmap(const QString& careerName);
    void onViewCareerDetails(const QString& careerName);

    // Roadmap slots
    void onRoadmapCareerChanged(int index);
    void onRoadmapTypeChanged();

private:
    void setupUI();
    QWidget* createSidebar();
    QWidget* createTopBar();
    QWidget* createProfilePage();
    QWidget* createStrategyPage();
    QWidget* createRecommendationsPage();
    QWidget* createRoadmapPage();

    void updateNavState(int activeIndex);
    void computeRecommendations();
    void renderRecommendationCards();
    void renderRoadmapSteps(const QString& careerName, bool isDegree);
    void appendTagToLineEdit(QLineEdit* lineEdit, const QString& tag);

    // Backend state
    CareerDatabase m_database;
    User m_user;
    std::vector<Recommendation> m_recommendations;
    int m_selectedStrategy; // 1 = Balanced, 2 = InterestFocused
    bool m_isDarkMode;

    // Navigation buttons
    QPushButton* m_btnNavProfile;
    QPushButton* m_btnNavStrategy;
    QPushButton* m_btnNavRecommendations;
    QPushButton* m_btnNavRoadmap;
    QStackedWidget* m_stackedWidget;

    // TopBar items
    QLabel* m_lblActiveUser;
    QPushButton* m_btnThemeToggle;

    // Profile form widgets
    QLineEdit* m_editName;
    QSpinBox* m_spinAge;
    QSlider* m_sliderAge;
    QLineEdit* m_editEducation;
    QLineEdit* m_editStream;
    QLineEdit* m_editInterests;
    QLineEdit* m_editSkills;
    QDoubleSpinBox* m_spinBudget;
    QLabel* m_lblProfileStatus;

    // Strategy widgets
    QRadioButton* m_radioBalanced;
    QRadioButton* m_radioInterest;
    QLabel* m_lblStrategyFormula;

    // Recommendations widgets
    QVBoxLayout* m_recsLayout;
    QLabel* m_lblRecsSummary;
    QWidget* m_recsCardsContainer;

    // Roadmap widgets
    QComboBox* m_comboRoadmapCareers;
    QPushButton* m_btnDegreePath;
    QPushButton* m_btnSkillPath;
    bool m_isDegreePathSelected;
    QVBoxLayout* m_roadmapTimelineLayout;
    QWidget* m_roadmapTimelineContainer;
    QLabel* m_lblRoadmapTitle;
    QLabel* m_lblRoadmapSubtitle;
};

#endif // MAINWINDOW_H

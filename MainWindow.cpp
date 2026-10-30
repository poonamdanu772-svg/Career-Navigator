#include "MainWindow.h"
#include "ThemeManager.h"

#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QTextBrowser>
#include <QSplitter>
#include <QProgressBar>
#include <QCheckBox>
#include <QHeaderView>
#include <algorithm>
#include <sstream>
#include <memory>

// Input validation matching backend specifications
static bool isValidName(const std::string& name)
{
    if (name.empty()) return false;
    bool hasLetter = false;
    for (unsigned char ch : name) {
        if (std::isalpha(ch)) hasLetter = true;
        else if (ch != ' ' && ch != '-' && ch != '\'') return false;
    }
    return hasLetter;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_selectedStrategy(1),
      m_isDarkMode(true),
      m_isDegreePathSelected(true)
{
    setWindowTitle("Career Navigator — Intelligent Career Guidance Platform");
    setMinimumSize(1150, 750);
    resize(1280, 820);

    // 1. Load career database from CSV
    if (!m_database.loadFromFile("careers.csv")) {
        QMessageBox::warning(this, "Database Warning",
            "Could not automatically locate 'careers.csv'. Ensure the CSV file is in the working directory.");
    }

    // 2. User profile starts unconfigured for user input
    // 3. Construct uniform UI
    setupUI();

    // 4. Set default executive dark theme
    qApp->setStyleSheet(ThemeManager::getDarkStyle());

    // 5. Initial state: wait for user to enter their profile
    renderRecommendationCards();
}

void MainWindow::setupUI()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left Sidebar
    mainLayout->addWidget(createSidebar());

    // Right Content Area (TopBar + Stacked Pages)
    auto* contentArea = new QWidget(this);
    auto* contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    contentLayout->addWidget(createTopBar());

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(createProfilePage());         // Index 0
    m_stackedWidget->addWidget(createStrategyPage());        // Index 1
    m_stackedWidget->addWidget(createRecommendationsPage()); // Index 2
    m_stackedWidget->addWidget(createRoadmapPage());         // Index 3

    contentLayout->addWidget(m_stackedWidget);
    mainLayout->addWidget(contentArea, 1);

    updateNavState(0);
}

QWidget* MainWindow::createSidebar()
{
    auto* sidebar = new QFrame(this);
    sidebar->setObjectName("sidebarFrame");
    sidebar->setFixedWidth(240);

    auto* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(16, 24, 16, 20);
    layout->setSpacing(8);

    // Brand Header
    auto* brandLayout = new QVBoxLayout();
    brandLayout->setSpacing(3);

    auto* lblTitle = new QLabel("CAREER NAVIGATOR", sidebar);
    lblTitle->setStyleSheet("font-size: 13px; font-weight: 800; color: #3b82f6; letter-spacing: 1.5px;");

    auto* lblSubtitle = new QLabel("Decision Support Platform", sidebar);
    lblSubtitle->setStyleSheet("font-size: 11px; color: #64748b; font-weight: 500;");

    brandLayout->addWidget(lblTitle);
    brandLayout->addWidget(lblSubtitle);
    layout->addLayout(brandLayout);

    layout->addSpacing(18);

    auto* lblNavHeader = new QLabel("NAVIGATION", sidebar);
    lblNavHeader->setStyleSheet("font-size: 10px; font-weight: 700; color: #475569; letter-spacing: 0.8px;");
    layout->addWidget(lblNavHeader);

    // Nav Buttons (Clean, Professional with Minimal Icons)
    m_btnNavProfile = new QPushButton("  👤  User Profile", sidebar);
    m_btnNavProfile->setObjectName("navBtn");
    connect(m_btnNavProfile, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
    layout->addWidget(m_btnNavProfile);

    m_btnNavStrategy = new QPushButton("  ⚖️  Scoring Strategy", sidebar);
    m_btnNavStrategy->setObjectName("navBtn");
    connect(m_btnNavStrategy, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
    layout->addWidget(m_btnNavStrategy);

    m_btnNavRecommendations = new QPushButton("  🎯  Career Matches", sidebar);
    m_btnNavRecommendations->setObjectName("navBtn");
    connect(m_btnNavRecommendations, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
    layout->addWidget(m_btnNavRecommendations);

    m_btnNavRoadmap = new QPushButton("  🗺️  Preparation Roadmaps", sidebar);
    m_btnNavRoadmap->setObjectName("navBtn");
    connect(m_btnNavRoadmap, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
    layout->addWidget(m_btnNavRoadmap);

    layout->addStretch();

    // Footer Info
    auto* lblVersion = new QLabel("v1.0 — Career Navigator", sidebar);
    lblVersion->setStyleSheet("font-size: 10px; color: #475569; padding: 8px 0;");
    layout->addWidget(lblVersion);

    return sidebar;
}

QWidget* MainWindow::createTopBar()
{
    auto* topBar = new QFrame(this);
    topBar->setObjectName("topBarFrame");
    topBar->setFixedHeight(58);

    auto* layout = new QHBoxLayout(topBar);
    layout->setContentsMargins(24, 0, 24, 0);

    auto* lblBreadcrumb = new QLabel("Career Navigator Platform", topBar);
    lblBreadcrumb->setStyleSheet("font-size: 13px; font-weight: 600; color: #94a3b8;");
    layout->addWidget(lblBreadcrumb);

    layout->addStretch();

    m_lblActiveUser = new QLabel("👤 Candidate: Not Configured", topBar);
    m_lblActiveUser->setObjectName("badgeLabel");
    layout->addWidget(m_lblActiveUser);

    layout->addSpacing(12);

    m_btnThemeToggle = new QPushButton("🌙 Dark Theme", topBar);
    m_btnThemeToggle->setObjectName("secondaryBtn");
    connect(m_btnThemeToggle, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    layout->addWidget(m_btnThemeToggle);

    return topBar;
}

void MainWindow::updateNavState(int activeIndex)
{
    QPushButton* buttons[] = {
        m_btnNavProfile, m_btnNavStrategy, m_btnNavRecommendations, m_btnNavRoadmap
    };

    for (int i = 0; i < 4; ++i) {
        buttons[i]->setProperty("active", (i == activeIndex) ? "true" : "false");
        buttons[i]->style()->unpolish(buttons[i]);
        buttons[i]->style()->polish(buttons[i]);
    }
}

void MainWindow::onNavButtonClicked()
{
    auto* senderBtn = qobject_cast<QPushButton*>(sender());
    if (!senderBtn) return;

    if (senderBtn == m_btnNavProfile) {
        m_stackedWidget->setCurrentIndex(0);
        updateNavState(0);
    } else if (senderBtn == m_btnNavStrategy) {
        m_stackedWidget->setCurrentIndex(1);
        updateNavState(1);
    } else if (senderBtn == m_btnNavRecommendations) {
        m_stackedWidget->setCurrentIndex(2);
        updateNavState(2);
    } else if (senderBtn == m_btnNavRoadmap) {
        m_stackedWidget->setCurrentIndex(3);
        updateNavState(3);
    }
}

void MainWindow::toggleTheme()
{
    m_isDarkMode = !m_isDarkMode;
    if (m_isDarkMode) {
        qApp->setStyleSheet(ThemeManager::getDarkStyle());
        m_btnThemeToggle->setText("🌙 Dark Theme");
    } else {
        qApp->setStyleSheet(ThemeManager::getLightStyle());
        m_btnThemeToggle->setText("☀️ Light Theme");
    }
}

// -------------------------------------------------------------
// USER PROFILE PAGE
// -------------------------------------------------------------
QWidget* MainWindow::createProfilePage()
{
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* pageWidget = new QWidget();
    auto* pageLayout = new QVBoxLayout(pageWidget);
    pageLayout->setContentsMargins(32, 28, 32, 28);
    pageLayout->setSpacing(20);

    // Header Card
    auto* bannerCard = new QFrame(pageWidget);
    bannerCard->setObjectName("cardFrame");
    auto* bannerLayout = new QVBoxLayout(bannerCard);
    bannerLayout->setContentsMargins(20, 18, 20, 18);
    bannerLayout->setSpacing(4);

    auto* lblBannerTitle = new QLabel("👤 User Profile & Qualifications", bannerCard);
    lblBannerTitle->setObjectName("headingLabel");

    auto* lblBannerSub = new QLabel(
        "Specify your academic background, core interests, and technical competencies to compute tailored career paths.", bannerCard);
    lblBannerSub->setObjectName("subheadingLabel");

    bannerLayout->addWidget(lblBannerTitle);
    bannerLayout->addWidget(lblBannerSub);
    pageLayout->addWidget(bannerCard);

    // Profile Templates Bar
    auto* presetsCard = new QFrame(pageWidget);
    presetsCard->setObjectName("cardFrame");
    auto* presetsLayout = new QVBoxLayout(presetsCard);
    presetsLayout->setContentsMargins(18, 14, 18, 14);
    presetsLayout->setSpacing(10);

    auto* lblPresets = new QLabel("Sample Profile Templates:", presetsCard);
    lblPresets->setStyleSheet("font-size: 12px; font-weight: 600; color: #94a3b8;");
    presetsLayout->addWidget(lblPresets);

    auto* btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);

    auto* btnDev = new QPushButton("💻 Software Engineering", presetsCard);
    btnDev->setObjectName("secondaryBtn");
    connect(btnDev, &QPushButton::clicked, this, &MainWindow::onApplyPresetSoftware);

    auto* btnDS = new QPushButton("📊 Data Science", presetsCard);
    btnDS->setObjectName("secondaryBtn");
    connect(btnDS, &QPushButton::clicked, this, &MainWindow::onApplyPresetDataScience);

    auto* btnDesign = new QPushButton("🎨 User Experience Design", presetsCard);
    btnDesign->setObjectName("secondaryBtn");
    connect(btnDesign, &QPushButton::clicked, this, &MainWindow::onApplyPresetDesign);

    auto* btnHealth = new QPushButton("🩺 Healthcare & Clinical", presetsCard);
    btnHealth->setObjectName("secondaryBtn");
    connect(btnHealth, &QPushButton::clicked, this, &MainWindow::onApplyPresetHealthcare);

    auto* btnBiz = new QPushButton("💼 Financial Services", presetsCard);
    btnBiz->setObjectName("secondaryBtn");
    connect(btnBiz, &QPushButton::clicked, this, &MainWindow::onApplyPresetBusiness);

    btnRow->addWidget(btnDev);
    btnRow->addWidget(btnDS);
    btnRow->addWidget(btnDesign);
    btnRow->addWidget(btnHealth);
    btnRow->addWidget(btnBiz);
    btnRow->addStretch();

    presetsLayout->addLayout(btnRow);
    pageLayout->addWidget(presetsCard);

    // Form Grid Container
    auto* formCard = new QFrame(pageWidget);
    formCard->setObjectName("cardFrame");
    auto* formLayout = new QGridLayout(formCard);
    formLayout->setContentsMargins(24, 24, 24, 24);
    formLayout->setHorizontalSpacing(24);
    formLayout->setVerticalSpacing(16);

    // Full Name
    auto* lblName = new QLabel("Full Name", formCard);
    lblName->setObjectName("formLabel");
    m_editName = new QLineEdit("", formCard);
    m_editName->setPlaceholderText("Enter your full name");
    formLayout->addWidget(lblName, 0, 0);
    formLayout->addWidget(m_editName, 1, 0);

    // Age
    auto* lblAge = new QLabel("Age (1 - 120)", formCard);
    lblAge->setObjectName("formLabel");

    auto* ageContainer = new QWidget(formCard);
    auto* ageLayout = new QHBoxLayout(ageContainer);
    ageLayout->setContentsMargins(0, 0, 0, 0);
    ageLayout->setSpacing(12);

    m_spinAge = new QSpinBox(ageContainer);
    m_spinAge->setRange(1, 120);
    m_spinAge->setValue(1);
    m_spinAge->setFixedWidth(80);
    m_spinAge->setSpecialValueText(" ");

    m_sliderAge = new QSlider(Qt::Horizontal, ageContainer);
    m_sliderAge->setRange(1, 60);
    m_sliderAge->setValue(1);

    connect(m_spinAge, QOverload<int>::of(&QSpinBox::valueChanged), m_sliderAge, &QSlider::setValue);
    connect(m_sliderAge, &QSlider::valueChanged, m_spinAge, &QSpinBox::setValue);

    ageLayout->addWidget(m_spinAge);
    ageLayout->addWidget(m_sliderAge);

    formLayout->addWidget(lblAge, 0, 1);
    formLayout->addWidget(ageContainer, 1, 1);

    // Education
    auto* lblEdu = new QLabel("Education Level", formCard);
    lblEdu->setObjectName("formLabel");
    m_editEducation = new QLineEdit("", formCard);
    m_editEducation->setPlaceholderText("Enter your education level (e.g. Bachelor of Technology)");
    formLayout->addWidget(lblEdu, 2, 0);
    formLayout->addWidget(m_editEducation, 3, 0);

    // Academic Stream
    auto* lblStream = new QLabel("Academic Stream", formCard);
    lblStream->setObjectName("formLabel");
    m_editStream = new QLineEdit("", formCard);
    m_editStream->setPlaceholderText("Enter your academic stream (e.g. Computer Science)");
    formLayout->addWidget(lblStream, 2, 1);
    formLayout->addWidget(m_editStream, 3, 1);

    // Interests
    auto* lblInterests = new QLabel("Interests (comma-separated)", formCard);
    lblInterests->setObjectName("formLabel");
    m_editInterests = new QLineEdit("", formCard);
    m_editInterests->setPlaceholderText("Enter interests (e.g. Programming, AI, Design, Finance, Healthcare)");
    formLayout->addWidget(lblInterests, 4, 0, 1, 2);
    formLayout->addWidget(m_editInterests, 5, 0, 1, 2);

    // Tag Suggestions
    auto* interestChips = new QWidget(formCard);
    auto* interestChipsLayout = new QHBoxLayout(interestChips);
    interestChipsLayout->setContentsMargins(0, 0, 0, 0);
    interestChipsLayout->setSpacing(6);

    const QStringList sampleInterests = {
        "Programming", "Problem Solving", "AI", "Design", "Data",
        "Healthcare", "Finance", "Gaming", "Robotics", "Research"
    };
    for (const auto& tag : sampleInterests) {
        auto* tagBtn = new QPushButton("+ " + tag, interestChips);
        tagBtn->setObjectName("tagBtn");
        tagBtn->setProperty("tagText", tag);
        connect(tagBtn, &QPushButton::clicked, this, &MainWindow::onInterestTagClicked);
        interestChipsLayout->addWidget(tagBtn);
    }
    interestChipsLayout->addStretch();
    formLayout->addWidget(interestChips, 6, 0, 1, 2);

    // Skills
    auto* lblSkills = new QLabel("Skills (comma-separated)", formCard);
    lblSkills->setObjectName("formLabel");
    m_editSkills = new QLineEdit("", formCard);
    m_editSkills->setPlaceholderText("Enter skills (e.g. C++, Python, Problem Solving, Communication, SQL)");
    formLayout->addWidget(lblSkills, 7, 0, 1, 2);
    formLayout->addWidget(m_editSkills, 8, 0, 1, 2);

    // Skill Tag Suggestions
    auto* skillChips = new QWidget(formCard);
    auto* skillChipsLayout = new QHBoxLayout(skillChips);
    skillChipsLayout->setContentsMargins(0, 0, 0, 0);
    skillChipsLayout->setSpacing(6);

    const QStringList sampleSkills = {
        "C++", "Python", "Java", "SQL", "Figma", "Excel",
        "Communication", "Logical Thinking", "Statistics", "Linux"
    };
    for (const auto& tag : sampleSkills) {
        auto* tagBtn = new QPushButton("+ " + tag, skillChips);
        tagBtn->setObjectName("tagBtn");
        tagBtn->setProperty("tagText", tag);
        connect(tagBtn, &QPushButton::clicked, this, &MainWindow::onSkillTagClicked);
        skillChipsLayout->addWidget(tagBtn);
    }
    skillChipsLayout->addStretch();
    formLayout->addWidget(skillChips, 9, 0, 1, 2);

    // Budget
    auto* lblBudget = new QLabel("Education / Training Budget", formCard);
    lblBudget->setObjectName("formLabel");
    m_spinBudget = new QDoubleSpinBox(formCard);
    m_spinBudget->setRange(0, 10000000);
    m_spinBudget->setValue(0.0);
    m_spinBudget->setPrefix("INR ");
    m_spinBudget->setSingleStep(5000);

    formLayout->addWidget(lblBudget, 10, 0, 1, 2);
    formLayout->addWidget(m_spinBudget, 11, 0, 1, 2);

    pageLayout->addWidget(formCard);

    // Action Row
    auto* actionContainer = new QHBoxLayout();
    actionContainer->setSpacing(16);

    m_lblProfileStatus = new QLabel("", pageWidget);
    m_lblProfileStatus->setStyleSheet("font-size: 13px; font-weight: 600; color: #10b981;");

    auto* btnSaveMatch = new QPushButton("Save Profile & Compute Matches", pageWidget);
    btnSaveMatch->setObjectName("primaryBtn");
    btnSaveMatch->setFixedHeight(40);
    btnSaveMatch->setCursor(Qt::PointingHandCursor);
    connect(btnSaveMatch, &QPushButton::clicked, this, &MainWindow::onSaveProfileAndMatch);

    actionContainer->addWidget(m_lblProfileStatus);
    actionContainer->addStretch();
    actionContainer->addWidget(btnSaveMatch);

    pageLayout->addLayout(actionContainer);
    pageLayout->addStretch();

    scrollArea->setWidget(pageWidget);
    return scrollArea;
}

void MainWindow::appendTagToLineEdit(QLineEdit* lineEdit, const QString& tag)
{
    QString current = lineEdit->text().trimmed();
    if (current.isEmpty()) {
        lineEdit->setText(tag);
    } else {
        QStringList parts = current.split(',');
        for (QString& p : parts) {
            if (p.trimmed().compare(tag, Qt::CaseInsensitive) == 0) return;
        }
        lineEdit->setText(current + ", " + tag);
    }
}

void MainWindow::onInterestTagClicked()
{
    auto* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        appendTagToLineEdit(m_editInterests, btn->property("tagText").toString());
    }
}

void MainWindow::onSkillTagClicked()
{
    auto* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        appendTagToLineEdit(m_editSkills, btn->property("tagText").toString());
    }
}

void MainWindow::onApplyPresetSoftware()
{
    m_spinAge->setValue(20);
    m_editEducation->setText("Bachelor of Technology");
    m_editStream->setText("Computer Science");
    m_editInterests->setText("Programming, AI, Web Development, Problem Solving, Building Products");
    m_editSkills->setText("C++, Python, Problem Solving, Data Structures, Git, Logical Thinking");
    m_spinBudget->setValue(50000.0);
}

void MainWindow::onApplyPresetDataScience()
{
    m_spinAge->setValue(21);
    m_editEducation->setText("Bachelor of Science");
    m_editStream->setText("Pure Sciences & Mathematics");
    m_editInterests->setText("Data, Patterns, Research, Business, Sharp Mind, Problem Solving");
    m_editSkills->setText("Python, SQL, Statistics, Excel, Data Visualization, Sharp Thinking");
    m_spinBudget->setValue(75000.0);
}

void MainWindow::onApplyPresetDesign()
{
    m_spinAge->setValue(22);
    m_editEducation->setText("Design degree or portfolio");
    m_editStream->setText("Design & Fine Arts");
    m_editInterests->setText("Design, Art, User Experience, Creativity, Visual Communication");
    m_editSkills->setText("Figma, Wireframing, Prototyping, Visual Design, User Research");
    m_spinBudget->setValue(40000.0);
}

void MainWindow::onApplyPresetHealthcare()
{
    m_spinAge->setValue(23);
    m_editEducation->setText("Medicine / MBBS");
    m_editStream->setText("Medicine & Healthcare");
    m_editInterests->setText("Healthcare, Science, Patient Care, Helping People, Medicine");
    m_editSkills->setText("Clinical Reasoning, Biology, Patient Monitoring, Empathy, Communication");
    m_spinBudget->setValue(200000.0);
}

void MainWindow::onApplyPresetBusiness()
{
    m_spinAge->setValue(21);
    m_editEducation->setText("Bachelor of Commerce");
    m_editStream->setText("Commerce & Accounting");
    m_editInterests->setText("Business, Finance, Investing, Problem Solving, Helping Others");
    m_editSkills->setText("Excel, Financial Analysis, Accounting, Communication, Analytical Thinking");
    m_spinBudget->setValue(60000.0);
}

void MainWindow::onSaveProfileAndMatch()
{
    std::string name = m_editName->text().trimmed().toStdString();
    if (!isValidName(name)) {
        QMessageBox::warning(this, "Validation Error",
            "Please enter a valid name using letters, spaces, and hyphens.");
        return;
    }

    int age = m_spinAge->value();
    if (age <= 1) {
        QMessageBox::warning(this, "Validation Error",
            "Please enter a valid age.");
        return;
    }

    std::string edu = m_editEducation->text().trimmed().toStdString();
    if (edu.empty()) {
        QMessageBox::warning(this, "Validation Error",
            "Please enter your education level.");
        return;
    }

    std::string stream = m_editStream->text().trimmed().toStdString();
    if (stream.empty()) {
        QMessageBox::warning(this, "Validation Error",
            "Please enter your academic stream.");
        return;
    }
    std::string interests = m_editInterests->text().trimmed().toStdString();
    std::string skills = m_editSkills->text().trimmed().toStdString();
    double budget = m_spinBudget->value();

    m_user.setName(name);
    m_user.setAge(age);
    m_user.setEducation(edu);
    m_user.setStream(stream);
    m_user.setInterests(interests);
    m_user.setSkills(skills);
    m_user.setBudget(budget);
    m_user.setCareerGoal("");

    m_lblActiveUser->setText(QString("👤 Candidate: %1 (%2) | %3")
        .arg(QString::fromStdString(name))
        .arg(age)
        .arg(QString::fromStdString(stream)));

    m_lblProfileStatus->setText("Profile saved successfully.");

    computeRecommendations();
    m_stackedWidget->setCurrentIndex(2);
    updateNavState(2);
}

// -------------------------------------------------------------
// SCORING STRATEGY PAGE
// -------------------------------------------------------------
QWidget* MainWindow::createStrategyPage()
{
    auto* pageWidget = new QWidget();
    auto* pageLayout = new QVBoxLayout(pageWidget);
    pageLayout->setContentsMargins(32, 28, 32, 28);
    pageLayout->setSpacing(20);

    // Banner
    auto* bannerCard = new QFrame(pageWidget);
    bannerCard->setObjectName("cardFrame");
    auto* bannerLayout = new QVBoxLayout(bannerCard);
    bannerLayout->setContentsMargins(20, 18, 20, 18);
    bannerLayout->setSpacing(4);

    auto* lblBannerTitle = new QLabel("⚖️ Scoring & Matching Strategy", bannerCard);
    lblBannerTitle->setObjectName("headingLabel");

    auto* lblBannerSub = new QLabel(
        "Choose how recommendation compatibility scores are weighted across education, interests, and skills.", bannerCard);
    lblBannerSub->setObjectName("subheadingLabel");

    bannerLayout->addWidget(lblBannerTitle);
    bannerLayout->addWidget(lblBannerSub);
    pageLayout->addWidget(bannerCard);

    // Strategy Selection Cards
    auto* strategiesLayout = new QHBoxLayout();
    strategiesLayout->setSpacing(16);

    // Balanced Model
    auto* cardBalanced = new QFrame(pageWidget);
    cardBalanced->setObjectName("cardFrame");
    auto* layoutBalanced = new QVBoxLayout(cardBalanced);
    layoutBalanced->setContentsMargins(20, 20, 20, 20);
    layoutBalanced->setSpacing(12);

    m_radioBalanced = new QRadioButton("Balanced Evaluation Model (Recommended)", cardBalanced);
    m_radioBalanced->setChecked(true);
    m_radioBalanced->setStyleSheet("font-size: 14px; font-weight: 700; color: #38bdf8;");
    connect(m_radioBalanced, &QRadioButton::toggled, this, &MainWindow::onStrategyChanged);

    auto* descBalanced = new QLabel(
        "Calculates an evenly weighted score balancing educational prerequisites, stated interests, and current technical skills.", cardBalanced);
    descBalanced->setWordWrap(true);
    descBalanced->setStyleSheet("color: #94a3b8; font-size: 13px; line-height: 1.4;");

    auto* progEdu1 = new QProgressBar(cardBalanced);
    progEdu1->setValue(20);
    progEdu1->setFormat("Education: 20%");

    auto* progInt1 = new QProgressBar(cardBalanced);
    progInt1->setValue(40);
    progInt1->setFormat("Interests: 40%");

    auto* progSkill1 = new QProgressBar(cardBalanced);
    progSkill1->setValue(40);
    progSkill1->setFormat("Skills: 40%");

    layoutBalanced->addWidget(m_radioBalanced);
    layoutBalanced->addWidget(descBalanced);
    layoutBalanced->addSpacing(4);
    layoutBalanced->addWidget(progEdu1);
    layoutBalanced->addWidget(progInt1);
    layoutBalanced->addWidget(progSkill1);
    layoutBalanced->addStretch();

    // Interest-Centric Model
    auto* cardInterest = new QFrame(pageWidget);
    cardInterest->setObjectName("cardFrame");
    auto* layoutInterest = new QVBoxLayout(cardInterest);
    layoutInterest->setContentsMargins(20, 20, 20, 20);
    layoutInterest->setSpacing(12);

    m_radioInterest = new QRadioButton("Interest-Centric Evaluation Model", cardInterest);
    m_radioInterest->setStyleSheet("font-size: 14px; font-weight: 700; color: #818cf8;");
    connect(m_radioInterest, &QRadioButton::toggled, this, &MainWindow::onStrategyChanged);

    auto* descInterest = new QLabel(
        "Prioritizes candidate interest alignment above existing qualification match. Suitable for career development exploration.", cardInterest);
    descInterest->setWordWrap(true);
    descInterest->setStyleSheet("color: #94a3b8; font-size: 13px; line-height: 1.4;");

    auto* progEdu2 = new QProgressBar(cardInterest);
    progEdu2->setValue(15);
    progEdu2->setFormat("Education: 15%");

    auto* progInt2 = new QProgressBar(cardInterest);
    progInt2->setValue(60);
    progInt2->setFormat("Interests: 60%");

    auto* progSkill2 = new QProgressBar(cardInterest);
    progSkill2->setValue(25);
    progSkill2->setFormat("Skills: 25%");

    layoutInterest->addWidget(m_radioInterest);
    layoutInterest->addWidget(descInterest);
    layoutInterest->addSpacing(4);
    layoutInterest->addWidget(progEdu2);
    layoutInterest->addWidget(progInt2);
    layoutInterest->addWidget(progSkill2);
    layoutInterest->addStretch();

    strategiesLayout->addWidget(cardBalanced);
    strategiesLayout->addWidget(cardInterest);
    pageLayout->addLayout(strategiesLayout);

    // Detail Box
    auto* infoBox = new QFrame(pageWidget);
    infoBox->setObjectName("cardFrame");
    auto* infoBoxLayout = new QVBoxLayout(infoBox);
    infoBoxLayout->setContentsMargins(20, 16, 20, 16);
    infoBoxLayout->setSpacing(6);

    auto* lblInfoTitle = new QLabel("Active Strategy Formula:", infoBox);
    lblInfoTitle->setStyleSheet("font-size: 12px; font-weight: 700; color: #38bdf8;");

    m_lblStrategyFormula = new QLabel(
        "Balanced Model: Score = (Education * 0.20) + (Interests * 0.40) + (Skills * 0.40)",
        infoBox
    );
    m_lblStrategyFormula->setStyleSheet("font-size: 13px; color: #cbd5e1;");

    infoBoxLayout->addWidget(lblInfoTitle);
    infoBoxLayout->addWidget(m_lblStrategyFormula);
    pageLayout->addWidget(infoBox);

    // Apply Button
    auto* btnApplyStrategy = new QPushButton("Apply Strategy & Re-Rank Recommendations", pageWidget);
    btnApplyStrategy->setObjectName("primaryBtn");
    btnApplyStrategy->setFixedHeight(40);
    connect(btnApplyStrategy, &QPushButton::clicked, [this]() {
        computeRecommendations();
        m_stackedWidget->setCurrentIndex(2);
        updateNavState(2);
    });

    pageLayout->addWidget(btnApplyStrategy, 0, Qt::AlignRight);
    pageLayout->addStretch();

    return pageWidget;
}

void MainWindow::onStrategyChanged()
{
    if (m_radioBalanced->isChecked()) {
        m_selectedStrategy = 1;
        m_lblStrategyFormula->setText(
            "Balanced Model: Score = (Education * 0.20) + (Interests * 0.40) + (Skills * 0.40)");
    } else {
        m_selectedStrategy = 2;
        m_lblStrategyFormula->setText(
            "Interest-Centric Model: Score = (Education * 0.15) + (Interests * 0.60) + (Skills * 0.25)");
    }
}

// -------------------------------------------------------------
// CAREER RECOMMENDATIONS PAGE
// -------------------------------------------------------------
QWidget* MainWindow::createRecommendationsPage()
{
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* pageWidget = new QWidget();
    auto* pageLayout = new QVBoxLayout(pageWidget);
    pageLayout->setContentsMargins(32, 28, 32, 28);
    pageLayout->setSpacing(20);

    // Header Card
    auto* bannerCard = new QFrame(pageWidget);
    bannerCard->setObjectName("cardFrame");
    auto* bannerLayout = new QHBoxLayout(bannerCard);
    bannerLayout->setContentsMargins(20, 16, 20, 16);

    auto* textCol = new QVBoxLayout();
    textCol->setSpacing(4);

    auto* lblBannerTitle = new QLabel("🎯 Career Recommendations", bannerCard);
    lblBannerTitle->setObjectName("headingLabel");

    m_lblRecsSummary = new QLabel("Highest ranked career opportunities based on current qualification criteria.", bannerCard);
    m_lblRecsSummary->setObjectName("subheadingLabel");

    textCol->addWidget(lblBannerTitle);
    textCol->addWidget(m_lblRecsSummary);
    bannerLayout->addLayout(textCol);
    bannerLayout->addStretch();

    auto* btnReRun = new QPushButton("🔄 Recalculate Matches", bannerCard);
    btnReRun->setObjectName("secondaryBtn");
    connect(btnReRun, &QPushButton::clicked, this, &MainWindow::computeRecommendations);
    bannerLayout->addWidget(btnReRun);

    pageLayout->addWidget(bannerCard);

    // Cards Layout
    m_recsCardsContainer = new QWidget(pageWidget);
    m_recsLayout = new QVBoxLayout(m_recsCardsContainer);
    m_recsLayout->setContentsMargins(0, 0, 0, 0);
    m_recsLayout->setSpacing(14);

    pageLayout->addWidget(m_recsCardsContainer);
    pageLayout->addStretch();

    scrollArea->setWidget(pageWidget);
    return scrollArea;
}

void MainWindow::computeRecommendations()
{
    BalancedRecommendationStrategy balancedStrategy;
    InterestFocusedStrategy interestStrategy;

    const RecommendationStrategy* strategyPtr = &balancedStrategy;
    if (m_selectedStrategy == 2) {
        strategyPtr = &interestStrategy;
    }

    RecommendationEngine engine;
    auto allRecs = engine.recommendCareers(m_user, m_database.getAllCareers(), *strategyPtr);

    m_recommendations.clear();
    for (const auto& item : allRecs) {
        bool dup = false;
        for (const auto& existing : m_recommendations) {
            if (QString::fromStdString(existing.career.getName()).toLower() ==
                QString::fromStdString(item.career.getName()).toLower()) {
                dup = true;
                break;
            }
        }
        if (!dup) {
            m_recommendations.push_back(item);
        }
    }

    const size_t MAX_RECS = 5;
    if (m_recommendations.size() > MAX_RECS) {
        m_recommendations.erase(m_recommendations.begin() + MAX_RECS, m_recommendations.end());
    }

    // Update Preparation Roadmaps dropdown with ONLY available recommended careers
    m_comboRoadmapCareers->blockSignals(true);
    m_comboRoadmapCareers->clear();
    for (const auto& rec : m_recommendations) {
        m_comboRoadmapCareers->addItem(QString::fromStdString(rec.career.getName()));
    }
    m_comboRoadmapCareers->blockSignals(false);

    if (m_comboRoadmapCareers->count() > 0) {
        renderRoadmapSteps(m_comboRoadmapCareers->currentText(), m_isDegreePathSelected);
    } else {
        renderRoadmapSteps("", m_isDegreePathSelected);
    }

    renderRecommendationCards();
}

void MainWindow::renderRecommendationCards()
{
    QLayoutItem* item;
    while ((item = m_recsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    if (m_recommendations.empty()) {
        auto* emptyCard = new QFrame(m_recsCardsContainer);
        emptyCard->setObjectName("cardFrame");
        auto* emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(30, 30, 30, 30);
        emptyLayout->setSpacing(8);

        auto* lblNone = new QLabel("No Career Matches Available", emptyCard);
        lblNone->setStyleSheet("font-size: 15px; font-weight: 600; color: #f59e0b;");
        auto* lblHint = new QLabel(
            "Please configure your profile under 'User Profile' and click 'Save Profile & Compute Matches' to generate your personalized career recommendations.", emptyCard);
        lblHint->setStyleSheet("font-size: 13px; color: #94a3b8;");

        emptyLayout->addWidget(lblNone);
        emptyLayout->addWidget(lblHint);
        m_recsLayout->addWidget(emptyCard);
        return;
    }

    for (size_t i = 0; i < m_recommendations.size(); ++i) {
        const auto& rec = m_recommendations[i];
        QString careerName = QString::fromStdString(rec.career.getName());
        QString description = QString::fromStdString(rec.career.getDescription());
        QString education = QString::fromStdString(rec.career.getRequiredEducation());
        double scorePct = rec.score * 100.0;

        auto* card = new QFrame(m_recsCardsContainer);
        card->setObjectName("cardFrame");
        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(20, 18, 20, 18);
        cardLayout->setSpacing(10);

        // Header
        auto* headerRow = new QHBoxLayout();
        headerRow->setSpacing(12);

        auto* lblRank = new QLabel(QString("Rank %1").arg(i + 1), card);
        lblRank->setObjectName("rankBadge");

        auto* lblName = new QLabel(careerName, card);
        lblName->setStyleSheet("font-size: 16px; font-weight: 700; color: #f8fafc;");

        auto* lblMatch = new QLabel(QString("%1% Match").arg(scorePct, 0, 'f', 1), card);
        lblMatch->setObjectName("matchBadge");

        headerRow->addWidget(lblRank);
        headerRow->addWidget(lblName);
        headerRow->addStretch();
        headerRow->addWidget(lblMatch);
        cardLayout->addLayout(headerRow);

        // Progress match bar
        auto* matchBar = new QProgressBar(card);
        matchBar->setValue(static_cast<int>(scorePct));
        matchBar->setTextVisible(false);
        matchBar->setFixedHeight(6);
        cardLayout->addWidget(matchBar);

        // Education requirement
        auto* lblEdu = new QLabel("Prerequisite: " + education, card);
        lblEdu->setStyleSheet("font-size: 12px; color: #38bdf8; font-weight: 500;");
        cardLayout->addWidget(lblEdu);

        // Description
        auto* lblDesc = new QLabel(description, card);
        lblDesc->setWordWrap(true);
        lblDesc->setStyleSheet("color: #94a3b8; font-size: 13px; line-height: 1.4;");
        cardLayout->addWidget(lblDesc);

        // Action Buttons Row
        auto* actionsRow = new QHBoxLayout();
        actionsRow->setSpacing(10);

        auto* btnRoadmap = new QPushButton("🗺️ View Preparation Roadmap", card);
        btnRoadmap->setObjectName("primaryBtn");
        btnRoadmap->setCursor(Qt::PointingHandCursor);
        connect(btnRoadmap, &QPushButton::clicked, [this, careerName]() {
            onExploreCareerRoadmap(careerName);
        });

        auto* btnDetails = new QPushButton("📋 Career Details", card);
        btnDetails->setObjectName("secondaryBtn");
        btnDetails->setCursor(Qt::PointingHandCursor);
        connect(btnDetails, &QPushButton::clicked, [this, careerName]() {
            onViewCareerDetails(careerName);
        });

        actionsRow->addWidget(btnRoadmap);
        actionsRow->addWidget(btnDetails);
        actionsRow->addStretch();
        cardLayout->addLayout(actionsRow);

        m_recsLayout->addWidget(card);
    }
}

void MainWindow::onExploreCareerRoadmap(const QString& careerName)
{
    int idx = m_comboRoadmapCareers->findText(careerName);
    if (idx != -1) {
        m_comboRoadmapCareers->setCurrentIndex(idx);
    } else {
        m_comboRoadmapCareers->addItem(careerName);
        m_comboRoadmapCareers->setCurrentIndex(m_comboRoadmapCareers->count() - 1);
    }

    renderRoadmapSteps(careerName, m_isDegreePathSelected);
    m_stackedWidget->setCurrentIndex(3);
    updateNavState(3);
}

void MainWindow::onViewCareerDetails(const QString& careerName)
{
    const Career* targetCareer = nullptr;
    for (const auto& c : m_database.getAllCareers()) {
        if (QString::fromStdString(c.getName()) == careerName) {
            targetCareer = &c;
            break;
        }
    }
    if (!targetCareer) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Career Specification — " + careerName);
    dlg.resize(550, 400);

    auto* layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(14);

    auto* lblTitle = new QLabel(careerName, &dlg);
    lblTitle->setStyleSheet("font-size: 17px; font-weight: 700; color: #f8fafc;");
    layout->addWidget(lblTitle);

    auto* lblDesc = new QLabel(QString::fromStdString(targetCareer->getDescription()), &dlg);
    lblDesc->setWordWrap(true);
    lblDesc->setStyleSheet("color: #cbd5e1; font-size: 13px; line-height: 1.4;");
    layout->addWidget(lblDesc);

    auto* lblEdu = new QLabel("Educational Requirement:\n" + QString::fromStdString(targetCareer->getRequiredEducation()), &dlg);
    lblEdu->setStyleSheet("font-size: 13px; color: #38bdf8; font-weight: 600;");
    layout->addWidget(lblEdu);

    QString skillsText = "Key Skill Areas: ";
    for (size_t i = 0; i < targetCareer->getRequiredSkills().size(); ++i) {
        skillsText += QString::fromStdString(targetCareer->getRequiredSkills()[i]);
        if (i + 1 < targetCareer->getRequiredSkills().size()) skillsText += ", ";
    }
    auto* lblSkills = new QLabel(skillsText, &dlg);
    lblSkills->setWordWrap(true);
    lblSkills->setStyleSheet("color: #94a3b8; font-size: 12px;");
    layout->addWidget(lblSkills);

    layout->addStretch();

    auto* btnClose = new QPushButton("Close", &dlg);
    btnClose->setObjectName("secondaryBtn");
    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignRight);

    dlg.exec();
}

// -------------------------------------------------------------
// CAREER PREPARATION ROADMAPS PAGE
// -------------------------------------------------------------
QWidget* MainWindow::createRoadmapPage()
{
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* pageWidget = new QWidget();
    auto* pageLayout = new QVBoxLayout(pageWidget);
    pageLayout->setContentsMargins(32, 28, 32, 28);
    pageLayout->setSpacing(20);

    // Banner
    auto* bannerCard = new QFrame(pageWidget);
    bannerCard->setObjectName("cardFrame");
    auto* bannerLayout = new QVBoxLayout(bannerCard);
    bannerLayout->setContentsMargins(20, 16, 20, 16);
    bannerLayout->setSpacing(4);

    auto* lblBannerTitle = new QLabel("🗺️ Career Preparation Roadmaps", bannerCard);
    lblBannerTitle->setObjectName("headingLabel");

    auto* lblBannerSub = new QLabel(
        "Structured milestone pathways designed for degree-oriented credentials and practical skill development.", bannerCard);
    lblBannerSub->setObjectName("subheadingLabel");

    bannerLayout->addWidget(lblBannerTitle);
    bannerLayout->addWidget(lblBannerSub);
    pageLayout->addWidget(bannerCard);

    // Controls Card
    auto* controlsCard = new QFrame(pageWidget);
    controlsCard->setObjectName("cardFrame");
    auto* controlsLayout = new QHBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 14, 20, 14);
    controlsLayout->setSpacing(16);

    auto* lblSelectCareer = new QLabel("Select Career:", controlsCard);
    lblSelectCareer->setStyleSheet("font-weight: 600; color: #e2e8f0;");
    controlsLayout->addWidget(lblSelectCareer);

    m_comboRoadmapCareers = new QComboBox(controlsCard);
    m_comboRoadmapCareers->setMinimumWidth(260);
    connect(m_comboRoadmapCareers, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onRoadmapCareerChanged);
    controlsLayout->addWidget(m_comboRoadmapCareers);

    controlsLayout->addStretch();

    // Pathway Segmented Buttons
    auto* pathToggleLayout = new QHBoxLayout();
    pathToggleLayout->setSpacing(4);

    m_btnDegreePath = new QPushButton("🎓 Degree Pathway", controlsCard);
    m_btnDegreePath->setObjectName("primaryBtn");
    m_btnDegreePath->setCursor(Qt::PointingHandCursor);

    m_btnSkillPath = new QPushButton("🛠️ Skill-Based Pathway", controlsCard);
    m_btnSkillPath->setObjectName("secondaryBtn");
    m_btnSkillPath->setCursor(Qt::PointingHandCursor);

    connect(m_btnDegreePath, &QPushButton::clicked, [this]() {
        m_isDegreePathSelected = true;
        m_btnDegreePath->setObjectName("primaryBtn");
        m_btnSkillPath->setObjectName("secondaryBtn");
        m_btnDegreePath->style()->unpolish(m_btnDegreePath);
        m_btnDegreePath->style()->polish(m_btnDegreePath);
        m_btnSkillPath->style()->unpolish(m_btnSkillPath);
        m_btnSkillPath->style()->polish(m_btnSkillPath);
        onRoadmapTypeChanged();
    });

    connect(m_btnSkillPath, &QPushButton::clicked, [this]() {
        m_isDegreePathSelected = false;
        m_btnSkillPath->setObjectName("primaryBtn");
        m_btnDegreePath->setObjectName("secondaryBtn");
        m_btnSkillPath->style()->unpolish(m_btnSkillPath);
        m_btnSkillPath->style()->polish(m_btnSkillPath);
        m_btnDegreePath->style()->unpolish(m_btnDegreePath);
        m_btnDegreePath->style()->polish(m_btnDegreePath);
        onRoadmapTypeChanged();
    });

    pathToggleLayout->addWidget(m_btnDegreePath);
    pathToggleLayout->addWidget(m_btnSkillPath);
    controlsLayout->addLayout(pathToggleLayout);

    pageLayout->addWidget(controlsCard);

    // Title Card
    auto* titleCard = new QFrame(pageWidget);
    titleCard->setObjectName("cardFrame");
    auto* titleCardLayout = new QVBoxLayout(titleCard);
    titleCardLayout->setContentsMargins(20, 16, 20, 16);
    titleCardLayout->setSpacing(4);

    m_lblRoadmapTitle = new QLabel("Preparation Roadmap", titleCard);
    m_lblRoadmapTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #ffffff;");

    m_lblRoadmapSubtitle = new QLabel("Degree Pathway Requirements", titleCard);
    m_lblRoadmapSubtitle->setStyleSheet("font-size: 12px; color: #94a3b8;");

    titleCardLayout->addWidget(m_lblRoadmapTitle);
    titleCardLayout->addWidget(m_lblRoadmapSubtitle);
    pageLayout->addWidget(titleCard);

    // Steps Timeline Container
    m_roadmapTimelineContainer = new QWidget(pageWidget);
    m_roadmapTimelineLayout = new QVBoxLayout(m_roadmapTimelineContainer);
    m_roadmapTimelineLayout->setContentsMargins(0, 0, 0, 0);
    m_roadmapTimelineLayout->setSpacing(12);

    pageLayout->addWidget(m_roadmapTimelineContainer);
    pageLayout->addStretch();

    scrollArea->setWidget(pageWidget);

    // Initial roadmap state
    renderRoadmapSteps("", true);

    return scrollArea;
}

void MainWindow::onRoadmapCareerChanged(int index)
{
    Q_UNUSED(index);
    if (m_comboRoadmapCareers->currentIndex() >= 0) {
        renderRoadmapSteps(m_comboRoadmapCareers->currentText(), m_isDegreePathSelected);
    }
}

void MainWindow::onRoadmapTypeChanged()
{
    if (m_comboRoadmapCareers->currentIndex() >= 0) {
        renderRoadmapSteps(m_comboRoadmapCareers->currentText(), m_isDegreePathSelected);
    }
}

void MainWindow::renderRoadmapSteps(const QString& careerName, bool isDegree)
{
    QLayoutItem* item;
    while ((item = m_roadmapTimelineLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    if (careerName.trimmed().isEmpty() || m_recommendations.empty()) {
        m_lblRoadmapTitle->setText("No Career Selected");
        m_lblRoadmapSubtitle->setText("Awaiting profile configuration");

        auto* emptyCard = new QFrame(m_roadmapTimelineContainer);
        emptyCard->setObjectName("cardFrame");
        auto* emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(24, 24, 24, 24);

        auto* lblNone = new QLabel("No career roadmap available yet.", emptyCard);
        lblNone->setStyleSheet("color: #94a3b8; font-size: 14px; font-weight: 600;");

        auto* lblSub = new QLabel("Please enter your profile details under 'User Profile' and compute your matches. Your recommended careers will appear here with full roadmap pathways.", emptyCard);
        lblSub->setStyleSheet("color: #64748b; font-size: 12px;");

        emptyLayout->addWidget(lblNone);
        emptyLayout->addWidget(lblSub);
        m_roadmapTimelineLayout->addWidget(emptyCard);
        return;
    }

    std::string careerStr = careerName.toStdString();

    std::unique_ptr<CareerPath> pathPtr;
    if (isDegree) {
        pathPtr = std::make_unique<DegreeCareerPath>(careerStr);
        m_lblRoadmapSubtitle->setText("Degree Pathway • Academic Coursework & Formal Qualifications");
    } else {
        pathPtr = std::make_unique<SkillBasedCareerPath>(careerStr);
        m_lblRoadmapSubtitle->setText("Skill-Based Pathway • Self-Paced Projects & Competencies");
    }

    m_lblRoadmapTitle->setText(QString("%1 Roadmap — %2")
        .arg(isDegree ? "Degree-Based" : "Skill-Based")
        .arg(careerName));

    Roadmap roadmap = pathPtr->getRoadmap("roadmap_data.csv");
    const auto& steps = roadmap.getSteps();

    if (steps.empty()) {
        auto* emptyCard = new QFrame(m_roadmapTimelineContainer);
        emptyCard->setObjectName("cardFrame");
        auto* emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(24, 24, 24, 24);

        auto* lblNone = new QLabel(
            QString("No specific records found for '%1' in the preparation database.")
                .arg(careerName), emptyCard);
        lblNone->setStyleSheet("color: #f59e0b; font-size: 13px; font-weight: 600;");
        emptyLayout->addWidget(lblNone);

        m_roadmapTimelineLayout->addWidget(emptyCard);
        return;
    }

    for (const Step& step : steps) {
        auto* stepCard = new QFrame(m_roadmapTimelineContainer);
        stepCard->setObjectName("cardFrame");
        auto* cardLayout = new QHBoxLayout(stepCard);
        cardLayout->setContentsMargins(18, 16, 18, 16);
        cardLayout->setSpacing(16);

        // Circular Step Number
        auto* numCircle = new QLabel(QString::number(step.getSnum()), stepCard);
        numCircle->setFixedSize(36, 36);
        numCircle->setAlignment(Qt::AlignCenter);
        numCircle->setStyleSheet(
            "background-color: #0f172a; color: #38bdf8; border: 2px solid #2563eb; border-radius: 18px; font-size: 14px; font-weight: 700;");
        cardLayout->addWidget(numCircle, 0, Qt::AlignTop);

        // Step Content
        auto* contentCol = new QVBoxLayout();
        contentCol->setSpacing(6);

        auto* lblTitle = new QLabel(QString::fromStdString(step.getTitle()), stepCard);
        lblTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #f8fafc;");

        auto* lblDesc = new QLabel(QString::fromStdString(step.getDescription()), stepCard);
        lblDesc->setWordWrap(true);
        lblDesc->setStyleSheet("color: #94a3b8; font-size: 13px; line-height: 1.4;");

        contentCol->addWidget(lblTitle);
        contentCol->addWidget(lblDesc);
        cardLayout->addLayout(contentCol, 1);

        // Status Checkbox
        auto* chkComplete = new QCheckBox("Completed", stepCard);
        chkComplete->setStyleSheet("font-size: 11px; color: #64748b;");
        cardLayout->addWidget(chkComplete, 0, Qt::AlignVCenter);

        m_roadmapTimelineLayout->addWidget(stepCard);
    }
}

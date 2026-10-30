#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>

class ThemeManager {
public:
    static QString getDarkStyle() {
        return R"(
            /* Global Application Styles - Executive Dark Theme */
            QWidget {
                background-color: #0f172a;
                color: #f1f5f9;
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
                font-size: 13px;
                selection-background-color: #2563eb;
                selection-color: #ffffff;
            }

            QMainWindow {
                background-color: #0f172a;
            }

            QFrame#sidebarFrame {
                background-color: #0b1120;
                border-right: 1px solid #1e293b;
            }

            QFrame#topBarFrame {
                background-color: #0b1120;
                border-bottom: 1px solid #1e293b;
            }

            QFrame#cardFrame {
                background-color: #1e293b;
                border: 1px solid #334155;
                border-radius: 8px;
            }

            QFrame#accentCardFrame {
                background-color: #1e293b;
                border: 1px solid #2563eb;
                border-radius: 8px;
            }

            /* Scroll Area */
            QScrollArea {
                border: none;
                background-color: transparent;
            }
            QScrollArea > QWidget > QWidget {
                background-color: transparent;
            }

            /* Scrollbars */
            QScrollBar:vertical {
                border: none;
                background-color: #0f172a;
                width: 8px;
                margin: 0;
            }
            QScrollBar::handle:vertical {
                background-color: #334155;
                min-height: 24px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #475569;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
            QScrollBar:horizontal {
                height: 8px;
                background: #0f172a;
            }
            QScrollBar::handle:horizontal {
                background: #334155;
                border-radius: 4px;
            }

            /* Sidebar Navigation Buttons */
            QPushButton#navBtn {
                background-color: transparent;
                color: #94a3b8;
                border: none;
                border-radius: 6px;
                text-align: left;
                padding: 10px 14px;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton#navBtn:hover {
                background-color: #1e293b;
                color: #f8fafc;
            }
            QPushButton#navBtn[active="true"] {
                background-color: #2563eb;
                color: #ffffff;
                font-weight: 600;
            }

            /* Primary Action Buttons */
            QPushButton#primaryBtn {
                background-color: #2563eb;
                color: #ffffff;
                border: 1px solid #1d4ed8;
                border-radius: 6px;
                padding: 9px 18px;
                font-size: 13px;
                font-weight: 600;
            }
            QPushButton#primaryBtn:hover {
                background-color: #1d4ed8;
            }
            QPushButton#primaryBtn:pressed {
                background-color: #1e40af;
            }

            /* Secondary Buttons */
            QPushButton#secondaryBtn {
                background-color: #1e293b;
                color: #cbd5e1;
                border: 1px solid #334155;
                border-radius: 6px;
                padding: 8px 16px;
                font-size: 12px;
                font-weight: 500;
            }
            QPushButton#secondaryBtn:hover {
                background-color: #334155;
                color: #ffffff;
                border-color: #475569;
            }
            QPushButton#secondaryBtn:pressed {
                background-color: #0f172a;
            }

            /* Tag Pill Buttons */
            QPushButton#tagBtn {
                background-color: #0f172a;
                color: #cbd5e1;
                border: 1px solid #334155;
                border-radius: 14px;
                padding: 4px 12px;
                font-size: 11px;
                font-weight: 500;
            }
            QPushButton#tagBtn:hover {
                background-color: #1e293b;
                color: #f8fafc;
                border-color: #2563eb;
            }
            QPushButton#tagBtn[selected="true"] {
                background-color: #1e3a8a;
                color: #bfdbfe;
                border: 1px solid #2563eb;
            }

            /* Input Fields */
            QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
                background-color: #0f172a;
                color: #f8fafc;
                border: 1px solid #334155;
                border-radius: 6px;
                padding: 8px 12px;
                font-size: 13px;
            }
            QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
                border: 1px solid #3b82f6;
                background-color: #0f172a;
            }
            QLineEdit:hover, QComboBox:hover {
                border-color: #475569;
            }

            /* ComboBox Dropdown */
            QComboBox::drop-down {
                border: none;
                width: 24px;
            }
            QComboBox QAbstractItemView {
                background-color: #1e293b;
                color: #f1f5f9;
                border: 1px solid #334155;
                selection-background-color: #2563eb;
                selection-color: #ffffff;
                border-radius: 4px;
                padding: 4px;
            }

            /* Sliders */
            QSlider::groove:horizontal {
                height: 6px;
                background: #334155;
                border-radius: 3px;
            }
            QSlider::sub-page:horizontal {
                background: #2563eb;
                border-radius: 3px;
            }
            QSlider::handle:horizontal {
                background: #ffffff;
                border: 2px solid #2563eb;
                width: 16px;
                margin: -5px 0;
                border-radius: 8px;
            }
            QSlider::handle:horizontal:hover {
                background: #e2e8f0;
            }

            /* Typography */
            QLabel {
                color: #cbd5e1;
            }
            QLabel#headingLabel {
                color: #ffffff;
                font-size: 18px;
                font-weight: 700;
                letter-spacing: -0.2px;
            }
            QLabel#subheadingLabel {
                color: #94a3b8;
                font-size: 13px;
            }
            QLabel#sectionTitle {
                color: #f8fafc;
                font-size: 14px;
                font-weight: 600;
            }
            QLabel#formLabel {
                color: #94a3b8;
                font-size: 12px;
                font-weight: 500;
            }
            QLabel#badgeLabel {
                background-color: #1e293b;
                color: #38bdf8;
                border: 1px solid #334155;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 11px;
                font-weight: 600;
            }
            QLabel#matchBadge {
                background-color: #064e3b;
                color: #34d399;
                border: 1px solid #059669;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 12px;
                font-weight: 700;
            }
            QLabel#rankBadge {
                background-color: #1e293b;
                color: #93c5fd;
                border: 1px solid #2563eb;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 11px;
                font-weight: 700;
            }

            /* Progress Bar */
            QProgressBar {
                background-color: #0f172a;
                border: 1px solid #334155;
                border-radius: 4px;
                text-align: center;
                color: #f1f5f9;
                font-size: 11px;
                font-weight: 600;
                height: 8px;
            }
            QProgressBar::chunk {
                background-color: #2563eb;
                border-radius: 3px;
            }

            /* Radio Buttons & Checkboxes */
            QRadioButton, QCheckBox {
                color: #e2e8f0;
                font-size: 13px;
                spacing: 8px;
            }
            QRadioButton::indicator, QCheckBox::indicator {
                width: 18px;
                height: 18px;
            }
            QRadioButton::indicator:unchecked, QCheckBox::indicator:unchecked {
                border: 2px solid #475569;
                border-radius: 9px;
                background-color: #0f172a;
            }
            QCheckBox::indicator:unchecked {
                border-radius: 4px;
            }
            QRadioButton::indicator:checked {
                border: 2px solid #2563eb;
                border-radius: 9px;
                background-color: #2563eb;
            }
            QCheckBox::indicator:checked {
                border: 2px solid #2563eb;
                border-radius: 4px;
                background-color: #2563eb;
            }

            /* Table View */
            QTableWidget, QTableView {
                background-color: #0f172a;
                border: 1px solid #334155;
                border-radius: 6px;
                gridline-color: #1e293b;
                color: #e2e8f0;
            }
            QHeaderView::section {
                background-color: #1e293b;
                color: #94a3b8;
                padding: 8px 10px;
                border: none;
                border-bottom: 1px solid #334155;
                font-weight: 600;
                font-size: 12px;
            }
            QTableWidget::item:selected {
                background-color: #1e3a8a;
                color: #ffffff;
            }
        )";
    }

    static QString getLightStyle() {
        return R"(
            /* Global Application Styles - Executive Light Theme */
            QWidget {
                background-color: #f8fafc;
                color: #0f172a;
                font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
                font-size: 13px;
                selection-background-color: #2563eb;
                selection-color: #ffffff;
            }

            QMainWindow {
                background-color: #f8fafc;
            }

            QFrame#sidebarFrame {
                background-color: #ffffff;
                border-right: 1px solid #e2e8f0;
            }

            QFrame#topBarFrame {
                background-color: #ffffff;
                border-bottom: 1px solid #e2e8f0;
            }

            QFrame#cardFrame {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
            }

            QFrame#accentCardFrame {
                background-color: #ffffff;
                border: 1px solid #2563eb;
                border-radius: 8px;
            }

            QScrollArea {
                border: none;
                background-color: transparent;
            }
            QScrollArea > QWidget > QWidget {
                background-color: transparent;
            }

            QScrollBar:vertical {
                border: none;
                background-color: #f1f5f9;
                width: 8px;
                margin: 0;
            }
            QScrollBar::handle:vertical {
                background-color: #cbd5e1;
                min-height: 24px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #94a3b8;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }

            QPushButton#navBtn {
                background-color: transparent;
                color: #475569;
                border: none;
                border-radius: 6px;
                text-align: left;
                padding: 10px 14px;
                font-size: 13px;
                font-weight: 500;
            }
            QPushButton#navBtn:hover {
                background-color: #f1f5f9;
                color: #0f172a;
            }
            QPushButton#navBtn[active="true"] {
                background-color: #2563eb;
                color: #ffffff;
                font-weight: 600;
            }

            QPushButton#primaryBtn {
                background-color: #2563eb;
                color: #ffffff;
                border: 1px solid #1d4ed8;
                border-radius: 6px;
                padding: 9px 18px;
                font-size: 13px;
                font-weight: 600;
            }
            QPushButton#primaryBtn:hover {
                background-color: #1d4ed8;
            }
            QPushButton#primaryBtn:pressed {
                background-color: #1e40af;
            }

            QPushButton#secondaryBtn {
                background-color: #ffffff;
                color: #334155;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 8px 16px;
                font-size: 12px;
                font-weight: 500;
            }
            QPushButton#secondaryBtn:hover {
                background-color: #f1f5f9;
                color: #0f172a;
            }

            QPushButton#tagBtn {
                background-color: #f1f5f9;
                color: #475569;
                border: 1px solid #e2e8f0;
                border-radius: 14px;
                padding: 4px 12px;
                font-size: 11px;
                font-weight: 500;
            }
            QPushButton#tagBtn:hover {
                background-color: #e2e8f0;
                color: #0f172a;
            }
            QPushButton#tagBtn[selected="true"] {
                background-color: #dbeafe;
                color: #1d4ed8;
                border: 1px solid #93c5fd;
            }

            QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
                background-color: #ffffff;
                color: #0f172a;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 8px 12px;
                font-size: 13px;
            }
            QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
                border: 1px solid #2563eb;
                background-color: #ffffff;
            }

            QLabel {
                color: #334155;
            }
            QLabel#headingLabel {
                color: #0f172a;
                font-size: 18px;
                font-weight: 700;
                letter-spacing: -0.2px;
            }
            QLabel#subheadingLabel {
                color: #64748b;
                font-size: 13px;
            }
            QLabel#sectionTitle {
                color: #0f172a;
                font-size: 14px;
                font-weight: 600;
            }
            QLabel#formLabel {
                color: #475569;
                font-size: 12px;
                font-weight: 500;
            }
            QLabel#badgeLabel {
                background-color: #f1f5f9;
                color: #0369a1;
                border: 1px solid #e2e8f0;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 11px;
                font-weight: 600;
            }
            QLabel#matchBadge {
                background-color: #dcfce7;
                color: #15803d;
                border: 1px solid #86efac;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 12px;
                font-weight: 700;
            }
            QLabel#rankBadge {
                background-color: #eff6ff;
                color: #1d4ed8;
                border: 1px solid #bfdbfe;
                border-radius: 6px;
                padding: 4px 10px;
                font-size: 11px;
                font-weight: 700;
            }

            QProgressBar {
                background-color: #e2e8f0;
                border: 1px solid #cbd5e1;
                border-radius: 4px;
                text-align: center;
                color: #0f172a;
                font-size: 11px;
                font-weight: 600;
                height: 8px;
            }
            QProgressBar::chunk {
                background-color: #2563eb;
                border-radius: 3px;
            }
        )";
    }
};

#endif // THEMEMANAGER_H

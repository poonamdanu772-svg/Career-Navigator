#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <fstream>
#include <vector>

#ifdef QT_CORE_LIB
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#endif

inline std::string resolveDataFilePath(const std::string& filename)
{
    // 1. Check if the file exists directly in current working directory
    {
        std::ifstream f(filename);
        if (f.good()) return filename;
    }

#ifdef QT_CORE_LIB
    // 2. Check next to the executable
    QString appDir = QCoreApplication::applicationDirPath();
    QString path1 = appDir + "/" + QString::fromStdString(filename);
    if (QFileInfo::exists(path1)) {
        return path1.toStdString();
    }

    // 3. Check in macOS app bundle Resources (App.app/Contents/Resources)
    QString path2 = appDir + "/../Resources/" + QString::fromStdString(filename);
    if (QFileInfo::exists(path2)) {
        return QDir::cleanPath(path2).toStdString();
    }

    // 4. Check parent directory of executable
    QString path3 = appDir + "/../" + QString::fromStdString(filename);
    if (QFileInfo::exists(path3)) {
        return QDir::cleanPath(path3).toStdString();
    }

    // 5. Check 3 levels up (e.g. build/bin or build/Debug)
    QString path4 = appDir + "/../../../" + QString::fromStdString(filename);
    if (QFileInfo::exists(path4)) {
        return QDir::cleanPath(path4).toStdString();
    }
#endif

    // 6. Hardcoded fallback to the project source directory if on developer machine
    std::string sourcePath = "/Users/chotadon/Downloads/Career-Navigator-main/" + filename;
    {
        std::ifstream f(sourcePath);
        if (f.good()) return sourcePath;
    }

    return filename;
}

#endif // FILEUTILS_H

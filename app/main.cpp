#include "mainwindow.h"

#include "playlistmanager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QTranslator>
#include <QUrl>

// QM_FILE_INSTALL_DIR should be defined from the CMakeLists file.
#ifndef QM_FILE_INSTALL_DIR
#define QM_FILE_INSTALL_DIR ":/i18n/"
#endif // QM_FILE_INSTALL_DIR

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QString qmDir;
#ifdef _WIN32
    qmDir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("translations");
#else
    qmDir = QT_STRINGIFY(QM_FILE_INSTALL_DIR);
#endif
    if (translator.load(QString("PineapplePictures_%1").arg(QLocale::system().name()), qmDir)) {
        a.installTranslator(&translator);
    }
    a.setApplicationName("Pineapple Pictures");
    a.setApplicationDisplayName(QCoreApplication::translate("main", "Pineapple Pictures"));

    // parse commandline arguments
    QCommandLineParser parser;
    parser.addPositionalArgument("File list", QCoreApplication::translate("main", "File list."));
    parser.addHelpOption();

    parser.process(a);

    MainWindow w;
    w.show();

    QStringList urlStrList = parser.positionalArguments();
    QList<QUrl> && urlList = PlaylistManager::convertToUrlList(urlStrList);

    if (!urlList.isEmpty()) {
        w.showUrls(urlList);
    }

    w.initWindowSize();

    return a.exec();
}

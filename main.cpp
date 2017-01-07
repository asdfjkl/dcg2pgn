#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QDataStream>
#include <iostream>
#include <QStringList>
#include <QDebug>
#include "chess/pgn_reader.h"
#include "chess/indexentry.h"
#include "chess/database.h"
#include "chess/pgn_printer.h"



int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("dcg2pgn");
    QCoreApplication::setApplicationVersion("v1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("dcg2pgn");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "DCG input file."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "pgn* output files."));


    QCommandLineOption dcgFileOption(QStringList() << "d" << "dcg-file",
                                     QCoreApplication::translate("main", "DCG input file <database.dcg>."),
                                     QCoreApplication::translate("main", "filename."));
    parser.addOption(dcgFileOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source dcg is args.at(0), destination filename is args.at(1)

    if (!parser.parse(QCoreApplication::arguments())) {
        QString errorMessage = parser.errorText();
        std::cout << errorMessage.toStdString() << std::endl;
        exit(0);
    }

    QString dcFileName = parser.value(dcgFileOption);
    if(dcFileName.endsWith(".dcg") || dcFileName.endsWith(".dcs") ||
            dcFileName.endsWith(".dci") || dcFileName.endsWith(".dcn")) {
        dcFileName = dcFileName.left(dcFileName.size()-4);
    }

    qDebug() << dcFileName;

    QFile dcgFile;
    QFile dciFile;
    QFile dcsFile;
    QFile dcnFile;
    dciFile.setFileName(QString(dcFileName).append(".dci"));
    dcgFile.setFileName(QString(dcFileName).append(".dcg"));
    dcsFile.setFileName(QString(dcFileName).append(".dcs"));
    dcnFile.setFileName(QString(dcFileName).append(".dcn"));
    qDebug() << dciFile.fileName();
    if(!dciFile.exists()) {
        std::cout << "Error: can't open .dci file." << std::endl;
        exit(0);
    }
    if(!dcsFile.exists()) {
        std::cout << "Error: can't open .dcs file." << std::endl;
        exit(0);
    }
    if(!dcnFile.exists()) {
        std::cout << "Error: can't open .dcn file." << std::endl;
        exit(0);
    }
    if(!dcgFile.exists()) {
        std::cout << "Error: can't open .dcg file." << std::endl;
        exit(0);
    }

    QString basename = QString(dcFileName);
    chess::Database *db = new chess::Database(basename);
    qDebug() << "loading index...";
    db->loadIndex();
    qDebug() << "loading names...";
    db->loadNames();
    qDebug() << "loading sites...";
    db->loadSites();
    //chess::Game *g = db->getGameAt(0);

    chess::PgnPrinter *pp = new chess::PgnPrinter();
    //QStringList *pgn = pp->printGame(g);
    //std::cout << pgn->join("\n").toStdString() << std::endl;

    // walk through the list of index entries
    // a) create new game and new pgn file
    // b) parse index info
    // c) get site and name info from QMaps
    // d) seek to offset in dcg file
    // e) parse game at offset
    // f) append game to pgn file

    // don't forget to unmap and close all files

    QFile fOut(dcFileName.append(".pgn"));
    bool success = false;
    if(fOut.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream s(&fOut);
        for(int i=0;i<db->countGames();i++) {
            chess::Game *g = db->getGameAt(i);
            QStringList *pgn = pp->printGame(g);
            for (int i = 0; i < pgn->size(); ++i) {
                s << pgn->at(i) << '\n';
            }
            delete g;
            s << '\n' << '\n';
        }
        success = true;
    } else {
      std::cerr << "error opening output file\n";
    }
    fOut.close();
    if(!success) {
        throw std::invalid_argument("Error writing file");
    }
    return 0;
}

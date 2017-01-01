QT += core
QT += gui

CONFIG += c++11

TARGET = dcg2pgn
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    chess/board.cpp \
    chess/database.cpp \
    chess/ecocode.cpp \
    chess/game.cpp \
    chess/game_node.cpp \
    chess/gui_printer.cpp \
    chess/move.cpp \
    chess/namebase.cpp \
    chess/pgn_printer.cpp \
    chess/pgn_reader.cpp \
    chess/polyglot.cpp \
    chess/sitebase.cpp \
    chess/indexentry.cpp \
    chess/byteutil.cpp \
    chess/dcgencoder.cpp \
    chess/dcgdecoder.cpp

HEADERS += \
    chess/board.h \
    chess/database.h \
    chess/ecocode.h \
    chess/game.h \
    chess/game_node.h \
    chess/gui_printer.h \
    chess/move.h \
    chess/namebase.h \
    chess/pgn_printer.h \
    chess/pgn_reader.h \
    chess/polyglot.h \
    chess/sitebase.h \
    chess/indexentry.h \
    chess/byteutil.h \
    chess/dcgencoder.h \
    chess/dcgdecoder.h

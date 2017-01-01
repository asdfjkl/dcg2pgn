#include "dcgdecoder.h"
#include <QDebug>
#include <QStack>

chess::DcgDecoder::DcgDecoder()
{
    //this->game = new chess::Game();
}

int chess::DcgDecoder::decodeLength(QByteArray *ba, int *index) {
    int idx = *index;
    quint8 len1 = ba->at(idx);
    qDebug() << "len1 is: " << len1;
    if(len1 < 127) {
        (*index)++;
        return int(len1);
    }
    if(len1 == 0x81) {
        quint8 len2 = ba->at(idx+1);
        *index += 2;
        return int(len2);
    }
    if(len1 == 0x82) {
        quint16 len2 = ba->at(idx+1) * 256 + ba->at(idx+2);
        *index+=3;
        return int(len2);
    }
    if(len1 == 0x83) {
        quint32 ret = ba->at(idx+1) * 256 * 256 + ba->at(idx+2) * 256 + ba->at(idx+1);
        *index+=4;
        return ret;
    }
    if(len1 == 0x84) {
        quint32 ret = ba->at(idx+1) * 256*256*256 +  ba->at(idx+2) * 256 * 256 + ba->at(idx+3) * 256 + ba->at(idx+1);
        *index+=5;
        return int(ret);
    }
    throw std::invalid_argument("length decoding called with illegal byte value");
}

chess::Game* chess::DcgDecoder::decodeGame(QByteArray *ba) {
    Game *g = new chess::Game();
    // to remember variations
    QStack<GameNode*> *game_stack = new QStack<GameNode*>();
    game_stack->push(g->getRootNode());
    GameNode* current = g->getRootNode();
    int idx = 0;
    bool error = false;
    // first check if we have a fen marker
    quint8 fenmarker = ba->at(idx);
    if(fenmarker == 0x01) {
        idx++;
        int len = this->decodeLength(ba, &idx);
        QByteArray fen = ba->mid(idx, len);
        QString fen_string = QString::fromUtf8(fen);
        chess::Board *b = new chess::Board(fen_string);
        g->getCurrentNode()->setBoard(b);
        idx += len;
    } else if(fenmarker != 0x00) {
        error = true;
        idx++;
    }
    while(idx < ba->length() && !error) {
        quint8 byte = ba->at(idx);
        // >= 0x84: we have a marker, not a move
        if(byte >= 0x84) {
            if(byte == 0x84) {
                // start of variation
            }
            else if(byte == 0x85) {
                // end of variation

            }
            else if(byte == 0x86) {
                // start of comment
            }
            else if(byte == 0x87) {
                // annotations follow
            } else {
                error = true;
            }
        } else {
            // we have a move, decode next two bytes

        }

    }
    return g;
}

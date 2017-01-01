#ifndef DCGDECODER_H
#define DCGDECODER_H

#include "chess/game.h"
#include <QByteArray>

namespace chess {

class DcgDecoder
{
public:
    DcgDecoder();
    ~DcgDecoder();
    Game* decodeGame(QByteArray *ba);
    int decodeLength(QByteArray *ba, int *idx);

private:
    Game* game;

};

}

#endif // DCGDECODER_H

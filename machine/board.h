#ifndef __BOARD__H__
#define __BOARD__H__

struct Board {

    enum class Revision {
        Black,
        White
    };

    static Revision getRevision();
};

#endif  //!__BOARD__H__

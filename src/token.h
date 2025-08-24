#include <bits/stdc++.h>

#pragma once
using namespace std;

namespace ntr::token {
    enum class Keyword {
        FN,
        RETURN
    };
    struct LPar {}; // (
    struct RPar {}; // )
    struct LCB {}; // {
    struct RCB {}; // }
    struct Semicolon {}; // ;
    struct Plus {}; // +
    struct Minus {}; // -
    struct Multi {}; // *
    struct Slash {}; // +
    struct Integer { 
        int32_t value;
    };
    struct Identifier {
        string value;
    };
    struct Equals {};
    struct endTok {};

    using Any = variant<
        LPar,
        RPar,
        LCB,
        RCB,
        Semicolon,
        Plus,
        Integer,
        Identifier,
        Equals,
        Keyword,
        Minus,
        Multi,
        Slash,
        endTok
    >;
};
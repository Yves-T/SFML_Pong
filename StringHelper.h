//
//  StringHelper.h
//  PongGame
//
//

#ifndef __PongGame__StringHelper__
#define __PongGame__StringHelper__

#include <iostream>
#include <sstream>

template <typename T>
std::string toString(const T &value) {
    std::stringstream stream;
    stream << value;
    return stream.str();
}

#endif /* defined(__PongGame__StringHelper__) */

//
// Created by os on 5/21/24.
//

#ifndef PROJECT_BASE_V1_1_CONSOLE_HPP
#define PROJECT_BASE_V1_1_CONSOLE_HPP

#include "../h/buffer.hpp"
#include "../h/memoryAllocator.hpp"

class Console {
private:
    static Bafer* input;
    static Bafer* output;

public:
    static void initIO();
    static char getCharIN();
    static char getCharOUT();
    static void putCharIN(char c);
    static void putCharOUT(char c);

    static bool isEmpty();
};


#endif //PROJECT_BASE_V1_1_CONSOLE_HPP

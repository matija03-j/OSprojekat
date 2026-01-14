//
// Created by os on 5/21/24.
//

#include "../h/console.hpp"

Bafer* Console::input = nullptr;
Bafer* Console::output = nullptr;

void Console::initIO() {
    input = new Bafer();
    output = new Bafer();
}

char Console::getCharIN() {
    return input->get();
}

char Console::getCharOUT() {
    return output->get();
}

void Console::putCharIN(char c) {
    input->put(c);
}

void Console::putCharOUT(char c) {
    output->put(c);
}

bool Console::isEmpty() {
    if (output->getCnt() == 0){
        return true;
    } else
        return false;
}
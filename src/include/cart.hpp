#ifndef CART_HPP
#define CART_HPP

#include "common.hpp"

class Cartridge {
private:
    u8 test = 0;

public:
    Cartridge() = default;
    ~Cartridge() = default;

    auto test_() -> void;    
};

#endif    //CART_HPP
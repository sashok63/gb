#ifndef CART_HPP
#define CART_HPP

#include "common.hpp"

class Cartridge
{
private:
    static const array<u8, 0x30> nintendo_logo;

public:
    Cartridge() = default;

    auto get_nintedo_logo() const -> const array<u8, 0x30> & { return nintendo_logo; }
};

#endif // CART_HPP
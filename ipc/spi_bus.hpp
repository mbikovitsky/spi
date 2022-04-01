#pragma once

#include "line.hpp"


/**
 * Represents the data lines of an SPI bus.
 */
struct SPIBus
{
    Line miso;
    Line mosi;
    Line sclk;
};

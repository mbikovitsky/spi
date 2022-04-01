#include "spi_slave.hpp"

#include <cstdint>
#include <thread>
#include <utility>


SPISlave::SPISlave(SPIBus bus, std::chrono::milliseconds const half_clock)
    : bus_(std::move(bus)), half_clock_(half_clock)
{
}

std::optional<std::uint8_t> SPISlave::transact(std::uint8_t const byte)
{
    std::uint8_t read_byte = 0;

    for (std::uint8_t current_bit = 0x80; current_bit; current_bit >>= 1)
    {
        if (byte & current_bit)
        {
            bus_.miso.set_high();
        }
        else
        {
            bus_.miso.set_low();
        }

        if (!bus_.sclk.wait_high(half_clock_ * 2))
        {
            return {};
        }

        if (bus_.mosi.read())
        {
            read_byte |= current_bit;
        }

        if (!bus_.sclk.wait_low(half_clock_ * 2))
        {
            return {};
        }
    }

    return read_byte;
}

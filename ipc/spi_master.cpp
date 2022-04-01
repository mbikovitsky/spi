#include "spi_master.hpp"

#include <cstdint>
#include <thread>
#include <utility>


SPIMaster::SPIMaster(SPIBus bus, std::chrono::milliseconds const half_clock)
    : bus_(std::move(bus)), half_clock_(half_clock)
{
    bus_.sclk.set_low();
}

std::uint8_t SPIMaster::transact(std::uint8_t byte)
{
    try
    {
        std::uint8_t read_byte = 0;

        for (std::uint8_t current_bit = 0x80; current_bit; current_bit >>= 1)
        {
            if (byte & current_bit)
            {
                bus_.mosi.set_high();
            }
            else
            {
                bus_.mosi.set_low();
            }

            std::this_thread::sleep_for(half_clock_);

            bus_.sclk.set_high();

            if (bus_.miso.read())
            {
                read_byte |= current_bit;
            }

            std::this_thread::sleep_for(half_clock_);

            bus_.sclk.set_low();
        }

        return read_byte;
    }
    catch (...)
    {
        bus_.sclk.set_low();
        throw;
    }
}

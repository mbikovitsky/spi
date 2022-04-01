#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>

#include "spi_bus.hpp"


/**
 * Represents an SPI master.
 *
 * @note    This implements SPI mode 0 (CPOL = 0, CPHA = 0),
 *          with MSB-first transfers.
 */
class SPIMaster
{
private:
    SPIBus                    bus_;
    std::chrono::milliseconds half_clock_;

public:
    /**
     * Initializes a new SPI master.
     *
     * @param bus           The bus lines to use.
     * @param half_clock    Half the clock cycle length.
     */
    SPIMaster(SPIBus bus, std::chrono::milliseconds half_clock);

    /**
     * Performs a transaction of a single byte on the wire.
     */
    std::uint8_t transact(std::uint8_t byte);

    /**
     * Transmits and receives a sequence of bytes on the wire.
     *
     * @param input_first   Iterator to the beginning of the range of bytes to transmit.
     * @param input_last    Iterator to the end of the range to transmit.
     * @param output_first  Iterator to the beginning of the range to store the received bytes.
     *
     * @remark The output range must be at least as large as the input range.
     */
    template <typename InputIt, typename OutputIt>
    void transact(InputIt input_first, InputIt input_last, OutputIt output_first)
    {
        std::transform(input_first, input_last, output_first, [this](auto const byte) {
            return transact(byte);
        });
    }
};

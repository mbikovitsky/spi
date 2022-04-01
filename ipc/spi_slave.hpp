#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include "spi_bus.hpp"


/**
 * Represents an SPI slave.
 *
 * @note    This implements SPI mode 0 (CPOL = 0, CPHA = 0),
 *          with 8-bit MSB-first transfers.
 */
class SPISlave
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
    SPISlave(SPIBus bus, std::chrono::milliseconds half_clock);

    /**
     * Performs a transaction of a single byte on the wire.
     *
     * @returns The received byte or nullopt if a timeout occurred.
     */
    std::optional<std::uint8_t> transact(std::uint8_t byte);

    /**
     * Transmits and receives a sequence of bytes on the wire.
     *
     * @param input_first   Iterator to the beginning of the range of bytes to transmit.
     * @param input_last    Iterator to the end of the range to transmit.
     * @param output_first  Iterator to the beginning of the range to store the received bytes.
     *
     * @returns true if all bytes were successfully transmitted.
     *
     * @remark The output range must be at least as large as the input range.
     */
    template <typename InputIt, typename OutputIt>
    bool transact(InputIt input_first, InputIt input_last, OutputIt output_first)
    {
        auto current_input  = input_first;
        auto current_output = output_first;
        for (; current_input != input_last; ++current_input, ++current_output)
        {
            auto const byte = transact(*current_input);
            if (!byte)
            {
                return false;
            }
            *current_output = *byte;
        }
        return true;
    }
};

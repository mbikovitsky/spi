#pragma once

#include <chrono>
#include <optional>

#include "event.hpp"


/**
 * Represents a data line.
 */
class Line
{
private:
    Event low_;  // Signaled when the line is in the low state.
    Event high_; // Signaled when the line is in the high state.

public:
    Line(Event low, Event high);

    /**
     * Sets the line to the high state.
     */
    void set_high();

    /**
     * Sets the line to the low state.
     */
    void set_low();

    /**
     * Waits until the line becomes low.
     */
    bool wait_low(std::optional<std::chrono::milliseconds> timeout);

    /**
     * Waits until the line becomes high.
     */
    bool wait_high(std::optional<std::chrono::milliseconds> timeout);

    /**
     * Waits for a rising edge (low-to-high transition) on the line.
     */
    bool wait_rising(std::optional<std::chrono::milliseconds> timeout);

    /**
     * Waits for a falling edge (high-to-low transition) on the line.
     */
    bool wait_falling(std::optional<std::chrono::milliseconds> timeout);
};

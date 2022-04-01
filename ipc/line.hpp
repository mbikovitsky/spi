#pragma once

#include "common.hpp"
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
     * Reads the current state of the line.
     *
     * @returns true if the line is high, false otherwise.
     */
    bool read();

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
    bool wait_low(Timeout timeout);

    /**
     * Waits until the line becomes high.
     */
    bool wait_high(Timeout timeout);
};

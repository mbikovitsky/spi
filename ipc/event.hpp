#pragma once

#include <chrono>
#include <optional>
#include <string>

#include <Windows.h>

#include "common.hpp"


/**
 * Represents a Windows event object.
 */
class Event
{
private:
    HANDLE event_;

public:
    /**
     * Creates or opens an event object.
     *
     * @param manual_reset  Indicates whether the event should be automatically reset
     *                      after a wait is satisfied.
     * @param initial_state Indicates whether the event is initially signaled.
     * @param name          Optional name for the event object.
     */
    Event(bool manual_reset, bool initial_state, std::optional<std::wstring> const & name);

    ~Event();

    Event(Event const &) = delete;
    Event & operator=(Event const &) = delete;

    Event(Event && other) noexcept;
    Event & operator=(Event && other) noexcept;

    /**
     * Signals the event.
     */
    void set();

    /**
     * Sets the event to the non-signaled state.
     */
    void reset();

    /**
     * Waits for the event to become signaled.
     *
     * @param timeout Optional timeout for the wait. If not specified,
     *                the wait does not time out.
     *
     * @returns true if the event was signaled within the allotted time,
                false if the wait timed-out.
     */
    bool wait(Timeout timeout);
};

#include "line.hpp"

#include <chrono>
#include <utility>

#include "util.hpp"


Line::Line(Event low, Event high) : low_(std::move(low)), high_(std::move(high)) {}

bool Line::read()
{
    using namespace std::literals::chrono_literals;

    return high_.wait(0ms);
}

void Line::set_high()
{
    low_.reset();
    high_.set();
}

void Line::set_low()
{
    high_.reset();
    low_.set();
}

bool Line::wait_low(Timeout const timeout) { return low_.wait(timeout); }

bool Line::wait_high(Timeout const timeout) { return high_.wait(timeout); }

bool Line::wait_rising(Timeout const timeout)
{
    static_assert(bool {} == false);
    return combined_timeout(
        timeout,
        [this](auto const t) { return wait_low(t); },
        [this](auto const t) { return wait_high(t); });
}

bool Line::wait_falling(Timeout const timeout)
{
    static_assert(bool {} == false);
    return combined_timeout(
        timeout,
        [this](auto const t) { return wait_high(t); },
        [this](auto const t) { return wait_low(t); });
}

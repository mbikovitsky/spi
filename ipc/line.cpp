#include "line.hpp"

#include <algorithm>
#include <chrono>
#include <optional>
#include <utility>


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

bool Line::wait_low(std::optional<std::chrono::milliseconds> const timeout)
{
    return low_.wait(timeout);
}

bool Line::wait_high(std::optional<std::chrono::milliseconds> const timeout)
{
    return high_.wait(timeout);
}

bool Line::wait_rising(std::optional<std::chrono::milliseconds> const timeout)
{
    using namespace std::literals::chrono_literals;

    if (!timeout)
    {
        wait_low(std::nullopt);
        wait_high(std::nullopt);
    }

    auto const start = std::chrono::steady_clock::now();
    if (!wait_low(timeout))
    {
        return false;
    }
    auto const elapsed = std::chrono::steady_clock::now() - start;

    auto const remaining_timeout =
        std::max(std::chrono::duration_cast<std::chrono::milliseconds>(*timeout - elapsed), 0ms);

    return wait_high(remaining_timeout);
}

bool Line::wait_falling(std::optional<std::chrono::milliseconds> const timeout)
{
    using namespace std::literals::chrono_literals;

    if (!timeout)
    {
        wait_high(std::nullopt);
        wait_low(std::nullopt);
    }

    auto const start = std::chrono::steady_clock::now();
    if (!wait_high(timeout))
    {
        return false;
    }
    auto const elapsed = std::chrono::steady_clock::now() - start;

    auto const remaining_timeout =
        std::max(std::chrono::duration_cast<std::chrono::milliseconds>(*timeout - elapsed), 0ms);

    return wait_low(remaining_timeout);
}

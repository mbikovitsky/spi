#pragma once

#include <algorithm>
#include <chrono>

#include "common.hpp"


template <typename C1, typename C2>
auto combined_timeout(Timeout timeout, C1 first, C2 second) -> decltype(second(timeout))
{
    using namespace std::literals::chrono_literals;

    if (!timeout)
    {
        if (!first(timeout))
        {
            return {};
        }
        return second(timeout);
    }

    auto const start = std::chrono::steady_clock::now();

    if (!first(timeout))
    {
        return {};
    }

    auto const elapsed = std::chrono::steady_clock::now() - start;

    auto const remaining_timeout =
        std::max(std::chrono::duration_cast<std::chrono::milliseconds>(*timeout - elapsed), 0ms);

    return second(remaining_timeout);
}

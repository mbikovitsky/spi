#include "event.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <Windows.h>


namespace {

HANDLE create_event(bool const                          manual_reset,
                    bool const                          initial_state,
                    std::optional<std::wstring> const & name)
{
    auto const handle =
        CreateEventW(nullptr, manual_reset, initial_state, name ? name->c_str() : nullptr);
    if (nullptr == handle)
    {
        throw std::runtime_error("CreateEventW failed");
    }
    return handle;
}

} // namespace


Event::Event(bool const                          manual_reset,
             bool const                          initial_state,
             std::optional<std::wstring> const & name)
    : event_(create_event(manual_reset, initial_state, name))
{
}

Event::~Event()
{
    if (nullptr != event_)
    {
        CloseHandle(event_);
    }
}

Event::Event(Event && other) noexcept : event_(std::exchange(other.event_, nullptr)) {}

Event & Event::operator=(Event && other) noexcept
{
    event_ = std::exchange(other.event_, nullptr);
    return *this;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Event::set()
{
    if (!SetEvent(event_))
    {
        throw std::runtime_error("SetEvent failed");
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Event::reset()
{
    if (!ResetEvent(event_))
    {
        throw std::runtime_error("ResetEvent failed");
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
bool Event::wait(std::optional<std::chrono::milliseconds> const timeout)
{
    DWORD milliseconds = INFINITE;
    if (timeout)
    {
        milliseconds = static_cast<DWORD>(
            std::min(static_cast<decltype(timeout)::value_type::rep>(INFINITE), timeout->count()));
    }

    switch (auto const wait_result = WaitForSingleObject(event_, milliseconds))
    {
    case WAIT_OBJECT_0:
        return true;

    case WAIT_TIMEOUT:
        return false;

    default:
        throw std::runtime_error("WaitForSingleObject failed");
    }
}

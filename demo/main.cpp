#include <chrono>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>

#define DOCOPT_HEADER_ONLY
#include <docopt.h>

#include <event.hpp>
#include <line.hpp>
#include <spi_bus.hpp>
#include <spi_master.hpp>
#include <spi_slave.hpp>


namespace {

auto create_bus(std::string_view const name)
{
    Line miso(Event(true, false, std::string(name) + "_MISO_LOW"),
              Event(true, false, std::string(name) + "_MISO_HIGH"));

    Line mosi(Event(true, false, std::string(name) + "_MOSI_LOW"),
              Event(true, false, std::string(name) + "_MOSI_HIGH"));

    Line sclk(Event(true, false, std::string(name) + "_SCLK_LOW"),
              Event(true, false, std::string(name) + "_SCLK_HIGH"));

    return SPIBus {.miso = std::move(miso), .mosi = std::move(mosi), .sclk = std::move(sclk)};
}

[[noreturn]] void run_master(SPIBus bus, Event ss, std::chrono::milliseconds const half_clock)
{
    auto master = SPIMaster(std::move(bus), half_clock);

    for (;;)
    {
        std::string line {};
        std::getline(std::cin, line);

        ss.set();
        master.transact(std::begin(line), std::end(line), std::begin(line));
        ss.reset();

        std::cout << line << std::endl;
    }
}

[[noreturn]] void run_slave(SPIBus bus, Event ss, std::chrono::milliseconds const half_clock)
{
    using namespace std::literals::chrono_literals;

    auto slave = SPISlave(std::move(bus), half_clock);

    for (;;)
    {
        ss.wait({});

        do
        {
            if (auto const byte_read = slave.transact('='))
            {
                std::cout << static_cast<char>(*byte_read);
            }
        } while (ss.wait(0ms));

        std::cout << std::endl;
    }
}

} // namespace


int main(int const argc, char const * const * const argv)
{
    static constexpr char usage[] = R"(SPI Demo.

    Usage:
        demo master <bus_name> <ss_name> [--half-clock=<ms>]
        demo slave <bus_name> <ss_name> [--half-clock=<ms>]
        demo (-h | --help)
        demo --version

    Options:
        -h --help           Show this screen.
        --version           Show version.
        --half-clock=<ms>   Half the clock cycle length, in milliseconds [default: 10].)";

    auto args = docopt::docopt(usage, {argv + 1, argv + argc}, true, "SPI Demo v0.1.0");

    auto bus = create_bus(args["<bus_name>"].asString());

    auto ss = Event(true, false, args["<ss_name>"].asString());

    auto const half_clock = std::chrono::milliseconds(args["--half-clock"].asLong());

    if (args["master"].asBool())
    {
        run_master(std::move(bus), std::move(ss), half_clock);
    }
    else
    {
        run_slave(std::move(bus), std::move(ss), half_clock);
    }
}

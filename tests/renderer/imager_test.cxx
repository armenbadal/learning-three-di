#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

import e3d;

using namespace e3d::renderer;
using namespace e3d::graphics;

namespace {
    std::vector<std::string> read_tokens(const std::filesystem::path& p)
    {
        std::ifstream in{p};
        std::vector<std::string> tokens;
        std::string token;
        while( in >> token )
            tokens.push_back(token);
        return tokens;
    }

    std::filesystem::path temp_path()
    {
        return std::filesystem::temp_directory_path() / "renderer_framebuffer_test.ppm";
    }
}

TEST_CASE("save_as_ppm writes colours row-major")
{
    framebuffer fb{2, 2};
    fb(0, 0) = colour{255, 0, 0, 255};
    fb(1, 0) = colour{0, 255, 0, 255};
    fb(0, 1) = colour{0, 0, 255, 255};
    fb(1, 1) = colour{255, 255, 255, 255};

    const auto path = temp_path();
    save_as_ppm(fb, path);

    const auto tokens = read_tokens(path);
    std::filesystem::remove(path);

    REQUIRE(tokens.size() == 16);

    CHECK(tokens[0] == "P3");
    CHECK(tokens[1] == "2");
    CHECK(tokens[2] == "2");
    CHECK(tokens[3] == "255");

    const std::vector<std::string> expected
        {"255", "0", "0", "0", "255", "0", "0", "0", "255", "255", "255", "255"};
    for( std::size_t i = 0; i < expected.size(); ++i )
        CHECK(tokens[4 + i] == expected[i]);
}

TEST_CASE("save_as_ppm cleared framebuffer is all black")
{
    const framebuffer fb{3, 1};

    const auto path = temp_path();
    save_as_ppm(fb, path);

    const auto tokens = read_tokens(path);
    std::filesystem::remove(path);

    REQUIRE(tokens.size() == 13);

    CHECK(tokens[0] == "P3");
    CHECK(tokens[1] == "3");
    CHECK(tokens[2] == "1");
    CHECK(tokens[3] == "255");

    for( std::size_t i = 4; i < tokens.size(); ++i )
        CHECK(tokens[i] == "0");
}

TEST_CASE("save_as_ppm reports open errors")
{
    const framebuffer fb{1, 1};

    CHECK_THROWS_AS(save_as_ppm(fb, std::filesystem::temp_directory_path()), std::runtime_error);
}

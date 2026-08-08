#include <catch2/catch_test_macros.hpp>

#include "utility.hxx"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace renderer;

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

TEST_CASE("save_as_ppm writes pixels row-major")
{
    framebuffer fb{2, 2};
    fb.set(0, 0, pixel{255, 0, 0, 255});
    fb.set(1, 0, pixel{0, 255, 0, 255});
    fb.set(0, 1, pixel{0, 0, 255, 255});
    fb.set(1, 1, pixel{255, 255, 255, 255});

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

TEST_CASE("save_as_ppm cleared framebuffer is all zeros")
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
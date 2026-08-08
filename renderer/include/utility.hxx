#pragma once 

#include "framebuffer.hxx"

#include <filesystem>

namespace renderer {

void save_as_ppm(const framebuffer& fb, std::filesystem::path p);

} // namespace renderer


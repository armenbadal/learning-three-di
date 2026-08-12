#pragma once 

#include "engine3d/renderer/framebuffer.hxx"

#include <filesystem>

namespace e3d::renderer {

void save_as_ppm(const framebuffer& fb, std::filesystem::path p);

} // namespace e3d::renderer

#pragma once 

#include "engine3d/renderer/framebuffer.hxx"

#include <filesystem>

namespace engine3d::renderer {

void save_as_ppm(const framebuffer& fb, std::filesystem::path p);

} // namespace engine3d::renderer

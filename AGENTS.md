# Repository Guidelines

## Project Structure & Module Organization

`engine3d` is a C++23 static library. Public interfaces live under
`include/engine3d/`, grouped by domain (`math`, `geometry`, `camera`, `scene`,
`renderer`, and `platform`); implementations mirror this layout under `src/`.
Keep public declarations in `.hxx` files and implementation in `.cxx` files.
`tests/` follows the same domain structure. Standalone learning examples are in
`demos/` (including `demos/opengl/`), while the integrated application is in
`showcase/`. GLSL assets belong in `shaders/` or `showcase/shaders/`.

## Build, Test, and Development Commands

The top-level build requires CMake 4.2, a C++23 compiler, vcpkg
(`VCPKG_ROOT` is read by `CMakeLists.txt`), Catch2 3, GLFW, and OpenGL. A
matching GCC 16/CMake/Ninja environment is available through `Dockerfile`.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/engine3d-tests
```

Use `cmake --build build --target showcase` or a specific demo target (for
example, `cube-demo`) when iterating on an executable. Reconfigure after
editing any `CMakeLists.txt` file.

## Coding Style & Naming Conventions

Follow the existing C++ style: four-space indentation, braces on their own
line, and no tabs. Use lowercase snake_case for files, functions, variables,
and types (`matrix4x4`, `software_renderer`); reserve PascalCase for external
types such as Catch2. Keep namespaces under `e3d`, place standard-library
includes before project includes, and use `#pragma once` in headers. Prefer
`constexpr`/`noexcept` where they accurately express behavior. No formatter or
linter configuration is committed, so match nearby code and keep changes
focused.

## Testing Guidelines

Tests use Catch2 and are registered as the `engine3d-tests` CTest test. Add
new tests in the matching `tests/<domain>/` directory, name files
`*_test.cxx`, and add them to the `engine3d-tests` source list in the root
`CMakeLists.txt`. Write behavior-focused `TEST_CASE` names, such as
`"vector3 division by zero throws"`. Run the full CTest command above before
submitting; no coverage threshold is configured.

## Commit & Pull Request Guidelines

Recent history uses short, descriptive subject lines, often in Armenian, with
no conventional-commit prefix. Use an equally concise imperative or descriptive
summary (for example, `ավելացնել rasterizer-ի edge-case թեսթեր`). Keep commits
single-purpose. Pull requests should state the behavior changed, list test
commands run, link relevant issues when available, and include screenshots or
a short recording for demo/showcase visual changes.

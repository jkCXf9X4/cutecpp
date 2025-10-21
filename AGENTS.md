# Repository Guidelines

## Project Structure & Module Organization
- Public headers live in `include/`; keep names aligned with a single responsibility per file.
- Implementations mirror header names under `src/` (for example, `src/socket.cpp` implements `include/socket.hpp`).
- Place reusable fixtures or static data under `assets/`, and add new tests to `tests/` using the `test_<feature>.cpp` naming convention.
- Root `CMakeLists.txt` already calls `add_subdirectory(tests)`, so register each new test target there.

## Build, Test, and Development Commands
- Configure the toolchain once with `cmake --preset vcpkg`; this wires in Ninja and `$VCPKG_ROOT`.
- Build the static library via `cmake --build build`, or rebuild a specific component with `cmake --build build --target <target-name>`.
- Execute the suite using `ctest --test-dir build --output-on-failure` to surface logs quickly during debugging.

## Coding Style & Naming Conventions
- Target C++23 with four-space indentation and brace alignment matching `include/log.hpp`.
- Use PascalCase for classes and snake_case for functions, variables, and file names.
- Prefer standard library utilities (`std::format`, `std::source_location`, `std::unique_ptr`) before custom helpers.
- Run `clang-format -i` (LLVM preset) on every touched C++ source and header before committing.

## Testing Guidelines
- Write unit coverage in `tests/test_<feature>.cpp`; keep fixtures isolated and ensure resources (sockets, files) are cleaned up.
- Register new executables in `tests/CMakeLists.txt` using `add_executable` and `add_test`.
- Use focused assertions, and favor deterministic ports or temporary files for integration scenarios.

## Commit & Pull Request Guidelines
- Follow the existing short, imperative commit subjects (for example, `Add logger sink`), keeping them under 72 characters.
- Summaries should note behavior changes, new dependencies, and required follow-up.
- In pull requests, link related issues, document verification steps (`cmake --build`, `ctest --test-dir build`), and include log excerpts or screenshots when altering logging.
- Update `version.txt` and `vcpkg` manifests when introducing dependency or version shifts, and flag reviewers when `logging_guidlines.md` changes.

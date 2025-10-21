# cutecpp

**cutecpp** is a lightweight C++23 logging library that bridges local applications with [Cutelog](https://github.com/busimus/cutelog?tab=readme-ov-file). It provides colorized console output, structured JSON payloads, and optional sinks for files or sockets so that you can forward telemetry to Cutelog dashboards without additional plumbing.

## Project Layout
- `include/` – Public headers (`log.hpp`, `log_level.hpp`) that define the logging API.
- `src/` – Implementations that mirror the headers, including the Cutelog socket transport.
- `tests/` – Unit and integration tests registered via `tests/CMakeLists.txt`.
- `assets/` – Reusable fixtures or static data for tests and demos.
- `AGENTS.md` – Contributor guide covering coding standards and workflow details.

## Build & Test
1. Configure dependencies (Ninja, vcpkg toolchain) once:
   ```sh
   cmake --preset vcpkg
   ```
2. Build the static library:
   ```sh
   cmake --build build
   ```
3. Run the test suite with verbose failure output:
   ```sh
   ctest --test-dir build --output-on-failure
   ```

## Quick Start
```cpp
#include <log.hpp>

int main()
{
    using namespace ssp4cpp::utils::log;

    Logger logger{"example", info}; // sets level of terminal output
    Logger::enable_file_sink("example.log");
    Logger::enable_socket_sink("127.0.0.1:19996"); // Cutelog default

    logger(info)("connected to {}", "Cutelog");
    logger(error)("bad state: {}", 42);
}
```
- Console output is colorized by severity; anything above `LogLevel::ext_trace` is serialized to JSON and forwarded to enabled sinks.
- Toggle sinks at runtime with `Logger::disable_file_sink()` or by skipping `enable_socket_sink()`.

## Contributing
- Follow the conventions documented in `AGENTS.md` (naming, formatting, commit style).
- Use short, imperative commit messages (for example, `Add json sink`), and document verification commands in pull requests.
- Update `vcpkg` manifests or `version.txt` when new dependencies or API changes ship.

## License
cutecpp is distributed under the terms of the MIT License. See `LICENSE` for details.

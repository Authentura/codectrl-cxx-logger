# codeCTRL C++ Logger Library

This is the logger library for C++. The earliest version of C++ that this library supports
is C++17.

## Build instructions

Use the `make` command for the following actions:

- `make configure`: To configure the library using
  meson.
- `make reconfigure`: To reconfigure the library using meson.
- `make test`: To run the tests of the library to ensure the library functions as
  intended.
- `make test_view_output`: Runs the tests of the library then immediately uses `less` to
  view the the test logs.

By default, the build output will be place inside `build`. `compile_commands.json` assumes
that the build output is inside `build` and will not work properly if the build output is
changed.

### Important information

For *nix systems (i.e. Linux, BSD, macOS) and MinGW:

- Requires `BOOST_STACKTRACE_USE_BACKTRACE` to be enabled.
- Requires `libdl` and `libbacktrace` to be linked (see `/tests` for more details on how
  to do this in a meson context).
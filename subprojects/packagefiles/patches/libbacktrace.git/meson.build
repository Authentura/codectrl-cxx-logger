project('libbacktrace', 'c')

run_command('./configure', capture: true, check: true)
run_command('make', capture: true, check: true)
configure_file(copy: true, input: 'libbacktrace.la', output: 'libbacktrace.la')

libbacktrace_includes = include_directories('.')

libbacktrace_dep = declare_dependency(link_args: ['-L' + meson.current_source_dir() + '/.libs', '-lbacktrace'])
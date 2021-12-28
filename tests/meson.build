log_send_test = executable(
    'log_send_test',
    'log_send_test.cpp',
    include_directories: include,
    dependencies: libbacktrace_dep,
    link_args: ['-ldl'],
    install: false
)

test('log_send_test', log_send_test)
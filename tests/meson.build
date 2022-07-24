test_dependencies = [dependencies]
link_args = []
cpp_args = []

if build_machine.system() != 'windows'
  test_dependencies = [libbacktrace_dep, thread_dep]
  link_args = ['-ldl']
  cpp_args = ['-w']
else
  link_args = ['-lole32', '-ldbgeng']
endif

log_send_test = executable(
    'log_send_test',
    'log_send_test.cpp',
    include_directories: include,
    dependencies: test_dependencies,
    link_args: link_args,
    cpp_args: cpp_args,
    install: false,
)

log_if_send_test = executable(
    'log_if_send_test',
    'log_if_send_test.cpp',
    include_directories: include,
    dependencies: test_dependencies,
    link_args: link_args,
    cpp_args: cpp_args,
    install: false,
)

log_if_env_test = executable(
    'log_if_env_test',
    'log_if_env_send_test.cpp',
    include_directories: include,
    dependencies: test_dependencies,
    link_args: link_args,
    cpp_args: cpp_args,
    install: false,
)

test('log_send_test', log_send_test)
test('log_if_send_test', log_if_send_test)
test('log_if_env_test', log_if_env_test)

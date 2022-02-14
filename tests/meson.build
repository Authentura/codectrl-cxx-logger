test_dependencies = []
link_args = []

if build_machine.system() != 'windows'
  test_dependencies = [libbacktrace_dep, thread_dep]
  link_args = ['-ldl']
else
  link_args = ['-lole32', '-ldbgeng']
endif

log_send_test = executable(
    'log_send_test',
    'log_send_test.cpp',
    include_directories: include,
    dependencies: test_dependencies,
    link_args: link_args,
    install: false
)

test('log_send_test', log_send_test)

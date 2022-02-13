project('codectrl-cxx-logger', 'cpp', 
    version: '0.1.0',
    license: 'MIT',
    meson_version: '>= 0.55.0',
    default_options: [
        'cpp_std=c++17', 
        'default_library=both'
    ]
)

emails = ['<contact@pwnctrl.com>', '<s.boyden@pwnctrl.com>']
description = 'The codeCTRL logger library for C++'

subdir('third_party')

include = [include_directories(
    third_party_includes + ['./include',]
)]

libbacktrace_dep = dependency('libbacktrace', required: false)
thread_dep = dependency('threads', required: true)

if get_option('buildtype') == 'debug' and build_machine.system() != 'windows'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_BACKTRACE', language: 'cpp')
elif get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_WINDBG', language: 'cpp')
endif

if build_machine.system() != 'windows'
    if not libbacktrace_dep.found() 
        libbacktrace_proj = subproject('libbacktrace')
        libbacktrace_dep = libbacktrace_proj.get_variable('libbacktrace_dep')
        include += libbacktrace_proj.get_variable('libbacktrace_includes')
    endif
endif

subdir('include')
subdir('tests')
project('codectrl-cxx-logger', 'cpp', 
    version: '0.1.0',
    license: 'MIT',
    meson_version: '>= 0.50.0',
    default_options: [
        'cpp_std=c++17', 
        'warning_level=3', 
        'default_library=both'
    ]
)

emails = ['<contact@pwnctrl.com>', '<s.boyden@pwnctrl.com>']
description = 'The codeCTRL logger library for C++'

subdir('third_party')

include = include_directories(
    third_party_includes + ['./include',]
)

if get_option('buildtype') == 'debug' and build_machine.system() != 'windows'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_ADDR2LINE', language: 'cpp')
elif get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_WINDBG', language: 'cpp')
endif

subdir('include')
subdir('tests')
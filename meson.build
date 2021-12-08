project('codectrl', 'cpp', 
    version: '0.1.0',
    license: 'MIT',
    meson_version: '>= 0.50.0',
    default_options: ['cpp_std=c++2a', 'warning_level=3']
)

emails = ['<contact@pwnctrl.com>', '<s.boyden@pwnctrl.com>']
description = 'The codeCTRL logger library for C++'

include = include_directories(['./include'])

subdir('include')
subdir('third_party')
subdir('src')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
    libraries: codectrl_logger,
    name: 'lib' + meson.project_name(),
    version: meson.project_version(),
    filebase: meson.project_name(),
    subdirs: 'pwnctrl',
    url: 'https://github.com/pwnCTRL/codectrl-cxx-logger',
    description: description
)
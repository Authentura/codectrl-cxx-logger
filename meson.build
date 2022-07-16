project('codectrl-cxx-logger', 'cpp', 
    version: '0.3.0',
    license: 'MIT',
    meson_version: '>= 0.55.0',
    default_options: [
        'cpp_std=c++17', 
        'default_library=both',
    ]
)

emails = ['<contact@authentura.com>', '<s.boyden@authentura.com>']
description = 'The codeCTRL logger library for C++'

codectrl_logger_includes = [include_directories('include')]

libbacktrace_dep = dependency('libbacktrace', required: false)
thread_dep       = dependency('threads', required: false)

boost_array_proj           = subproject('boost_array')
boost_assert_proj          = subproject('boost_assert')
boost_config_proj          = subproject('boost_config')
boost_container_hash_proj  = subproject('boost_container_hash')
boost_core_proj            = subproject('boost_core')
boost_detail_proj          = subproject('boost_detail')
boost_integer_proj         = subproject('boost_integer')
boost_predef_proj          = subproject('boost_predef')
boost_stacktrace_proj      = subproject('boost_stacktrace')
boost_static_assert_proj   = subproject('boost_static_assert')
boost_throw_exception_proj = subproject('boost_throw_exception')
boost_type_index_proj      = subproject('boost_type_index')
boost_type_traits_proj     = subproject('boost_type_traits')
cps_proj                   = subproject('protobuf-specifications')

codectrl_logger_includes += [
    boost_array_proj.get_variable('boost_array_includes'),
    boost_assert_proj.get_variable('boost_assert_includes'),
    boost_config_proj.get_variable('boost_config_includes'),
    boost_container_hash_proj.get_variable('boost_container_hash_includes'),
    boost_core_proj.get_variable('boost_core_includes'),
    boost_detail_proj.get_variable('boost_detail_includes'),
    boost_integer_proj.get_variable('boost_integer_includes'),
    boost_predef_proj.get_variable('boost_predef_includes'),
    boost_stacktrace_proj.get_variable('boost_stacktrace_includes'),
    boost_static_assert_proj.get_variable('boost_static_assert_includes'),
    boost_throw_exception_proj.get_variable('boost_throw_exception_includes'),
    boost_type_index_proj.get_variable('boost_type_index_includes'),
    boost_type_traits_proj.get_variable('boost_type_traits_includes'),
    cps_proj.get_variable('cps_includes')
]

if get_option('buildtype') == 'debug' and build_machine.system() != 'windows'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_BACKTRACE', language: 'cpp')
elif get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG', '-DBOOST_STACKTRACE_USE_WINDBG', language: 'cpp')
endif

dependencies = []

if build_machine.system() != 'windows'
    if not libbacktrace_dep.found() 
        libbacktrace_proj = subproject('libbacktrace')
        libbacktrace_dep = libbacktrace_proj.get_variable('libbacktrace_dep')
        codectrl_logger_includes += libbacktrace_proj.get_variable('libbacktrace_includes')
    endif

    dependencies = [libbacktrace_dep, thread_dep, ]
endif

include = codectrl_logger_includes

subdir('include')
subdir('tests')

codectrl_dep = declare_dependency(dependencies: dependencies)

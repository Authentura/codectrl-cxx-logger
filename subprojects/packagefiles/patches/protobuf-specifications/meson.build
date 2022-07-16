project('codectrl-protobuf-specifications', 'cpp')

cmake = import('cmake')

grpc_options = cmake.subproject_options()
grpc_options.add_cmake_defines(
    { 
        'CMAKE_INSTALL_PREFIX': meson.current_source_dir() + 'out',
        'gRPC_BUILD_GRPC_CSHARP_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_NODE_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_PHP_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_PYTHON_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_RUBY_PLUGIN': 'OFF',
    }
)
grpc_options.set_install(true, target: 'plugins')
grpc_options.set_install(true, target: 'tools_cxx')
grpc_options.set_install(true)
grpc_proj = cmake.subproject('grpc', options: grpc_options)

plugins = library('grpc_plugins', grpc_proj.target('plugins'))

protoc = find_program('protoc')
python = find_program('python3')

if not python.found()
  python = find_program('python')
endif

run_command(
  ['mkdir', '-p', 'include'], 
  check: true
)

glob = run_command(python, 'glob', check: true)
sources = glob.stdout().strip().split('\n')

generator(
  protoc, 
  arguments: [
    '-I.', 
    '-I/usr/include', 
    '--cpp_out=includes', 
    '--grpc_out=includes', 
    sources
  ], 
  capture: true, 
  depends: plugins
)

cps_includes = [
    include_directories('include'), 
    include_directories(meson.source_dir() + 'subprojects/grpc.git/include')
]
cps_dependency = grpc_proj.dependency('grpc')

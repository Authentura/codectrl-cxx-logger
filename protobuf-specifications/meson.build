cmake = import('cmake')

grpc_options = cmake.subproject_options()
grpc_options.add_cmake_defines(
    { 
        'gRPC_BUILD_GRPC_CSHARP_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_NODE_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_OBJECTIVE_C_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_PHP_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_PYTHON_PLUGIN': 'OFF',
        'gRPC_BUILD_GRPC_RUBY_PLUGIN': 'OFF',
        'gRPC_BUILD_TESTS': 'OFF',
        'gRPC_BUILD_CODEGEN': 'OFF'
    }
)
grpc_options.set_install(true, target: 'plugins')
grpc_options.set_install(true, target: 'tools_cxx')

grpc_proj  = cmake.subproject('grpc', options: grpc_options)
specs_proj = subproject('protobuf-specifications')

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

input = run_command(
  [python, 'inputs', specs_proj.get_variable('proto_directory')],
  check: true,
  capture: true
)
inputs = input.stdout().strip().split('\n')

output = run_command(
  [python, 'outputs', specs_proj.get_variable('proto_directory')],
  check: true,
  capture: true
)
outputs = output.stdout().strip().split('\n')

message('CMaket targets:\n - ' + '\n - '.join(grpc_proj.target_list()))

# grpc_cpp_generator = executable(
#     grpc_proj.target('plugins'), 
#     ['grpc_cpp_plugin'], 
#     dependencies: [grpc_proj.dependency()]
# )

# if not grpc_cpp_generator.found()
#   message('Could not find grpc_cpp_generator')
# endif

cps_sources = custom_target(
  input: inputs,
  output: outputs,
  command: [
    protoc, 
    '-I' + specs_proj.get_variable('proto_directory'), 
    '-I/usr/include', 
    '--cpp_out=' + meson.current_source_dir() + 'include', 
    '--grpc_out=' + meson.current_source_dir() + 'include', 
    '@INPUT@'
  ], 
  depends: plugins,
)

subdir('include')

cps_dependency = grpc_proj.dependency('grpc')

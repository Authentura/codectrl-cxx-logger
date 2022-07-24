project('codectrl-protobuf-specifications', 'c')

protoc = find_program('protoc')
python = find_program('python3')

if not python.found()
  python = find_program('python')
endif

glob = run_command(python, 'glob', check: true)
sources = glob.stdout().strip().split('\n')

proto_directory = meson.current_source_dir()
proto_files = []

foreach file: files(sources)
  proto_files += '@0@'.format(file)
endforeach

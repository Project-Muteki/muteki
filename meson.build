project('muteki', 'c',
    version : '5.0.0',
    default_options : ['warning_level=3'])

c_flags = ['-mcpu=arm7tdmi', '-ffreestanding']
ld_flags = c_flags

# Define __MUTEKI_IS_BOOTSTRAPPING__ for the entire project when sys/types.h is missing.
c = meson.get_compiler('c')
if not c.has_header('sys/types.h')
    add_project_arguments(['-D__MUTEKI_IS_BOOTSTRAPPING__=1'], language: 'c')
endif

syscall_files_sdk = ['syscall_def/syscalls_sdk.json', 'syscall_def/syscalls_sdk_hpprime.json']
syscall_files_krnl = ['syscall_def/syscalls_krnl.json', 'syscall_def/syscalls_krnl_hpprime.json', 'syscall_def/syscalls_krnl_pocketchallenge.json']

# For gen_syscall_shim.py.
python_interp = import('python').find_installation('python3')

syscalls_krnl = custom_target(
    'syscalls_krnl.s',
    output : 'syscalls_krnl.s',
    input : ['scripts/gen_syscall_shim.py', syscall_files_krnl],
    command: [python_interp, '@INPUT0@', '-o', '@OUTPUT@', '@INPUT1@', '@INPUT2@'],
)

syscalls_sdk = custom_target('syscalls_sdk.s',
    output : 'syscalls_sdk.s',
    input : ['scripts/gen_syscall_shim.py', syscall_files_sdk],
    command: [python_interp, '@INPUT0@', '-o', '@OUTPUT@', '@INPUT1@', '@INPUT2@'],)

_syscall_table = run_command([python_interp, 'scripts/gen_syscall_shim.py',
    '-Sd',
    'syscall_def/syscalls_sdk.json', 'syscall_def/syscalls_sdk_hpprime.json',
    'syscall_def/syscalls_krnl.json', 'syscall_def/syscalls_krnl_hpprime.json',
    'syscall_def/syscalls_krnl_pocketchallenge.json'],
    check : true)

_syscall_split_files = _syscall_table.stdout().strip().split('\n')

syscalls_split = custom_target('syscalls_split',
    output : _syscall_split_files,
    input : ['scripts/gen_syscall_shim.py', syscall_files_sdk, syscall_files_krnl],
    command: [python_interp, '@INPUT0@', '-t', 'gas-eabi', '-So', '@BUILD_ROOT@', '@INPUT1@', '@INPUT2@', '@INPUT3@', '@INPUT4@', '@INPUT5@'],)

# Static library that bundles both sdk and krnl shims.
static_library(
    'muteki-shims',
    [syscalls_split, 'src/shims/syscall.c'],
    include_directories: ['include/'],
    install : true,
    c_args : c_flags,
    link_args : ld_flags,
    pic: false,
)

osdep_src = [
    'src/osdep/threading.c',
    'src/osdep/ktls.c',
    'src/osdep/utls.c',
    'src/osdep/heap.c',
]

static_library(
    'muteki-osdep',
    osdep_src,
    include_directories: ['include/'],
    install : true,
    c_args : c_flags,
    link_args : ld_flags,
    pic : false,
)

if get_option('generate_dll_replicas')

# Need this for "bare metal" system or "none" OS since it will use UNIX/ELF-style naming otherwise.
lib_suffix_override = 'dll'
lib_prefix_override = ''

# sdklib replica
shared_library(
    'sdklib',
    'src/dll_entry_stub.s',
    syscalls_sdk,
    install : true,
    c_args : c_flags,
    link_args : ld_flags,
    name_suffix : lib_suffix_override,
    name_prefix : lib_prefix_override,
)

# krnllib replica
shared_library(
    'krnllib',
    'src/dll_entry_stub.s',
    syscalls_krnl,
    install : true,
    c_args : c_flags,
    link_args : ld_flags,
    name_suffix : lib_suffix_override,
    name_prefix : lib_prefix_override,
)

endif # get_option('generate_dll_replicas')

local_includes = include_directories('include')

executable(
    'test-build-c',
    'src/test.c',
    include_directories: local_includes,
    install: false,
    build_by_default: false,
)

# Headers for the shims
install_headers('include/muteki.h')
install_subdir('include/muteki/',
    install_dir : 'include/')
install_subdir('include/osdep/',
    install_dir : 'include/')

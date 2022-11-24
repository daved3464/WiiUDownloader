project('WiiUDownloader', ['c', 'cpp'],
    version: '0.1',
    default_options : [ 'c_std=c99', 'cpp_std=c++20'])

env = environment()
compiler = meson.get_compiler('c')
gnome = import('gnome')

host_system = host_machine.system()
build_system = build_machine.system()
target_system = target_machine.system()

c_args = [
    '-D_GNU_SOURCE',    
    '-DAES_ROM_TABLES',    
    '-DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED',
    '-fno-strict-aliasing',
    '-pipe',
]

cpp_args = [ 
    '-D_GNU_SOURCE',    
    '-DAES_ROM_TABLES',
    '-DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED',
    '-fno-strict-aliasing',
    '-pipe',
    '-Wall',
]

if (host_system == 'windows' and 
    meson.get_compiler('c').get_id() == 'gcc')
    c_args += ['-static-libc']
    cpp_args += ['-static-libstdc++']
endif

if get_option('release')
    c_args += ['-fvisibility=hidden', '-Ofast']
    cpp_args += ['-fvisibility=hidden', '-Ofast']
endif

if get_option('clang')
    clang_c = find_program(['clang'], required: true)
    clang_cc = find_program(['clang++'], required: true)

    env.set('CC', clang_c.full_path())    
    env.set('CXX', clang_cc.full_path())
endif

if get_option('mold')
    mold = find_program(['mold'], required: true)

    env.set('CC_LD', mold.full_path())
    env.set('CXX_LD', mold.full_path())
endif

src =  []

dependencies = [
    compiler.find_library('mbedtls'),
    compiler.find_library('mbedcrypto'),
    compiler.find_library('mbedx509'),    
    dependency('libcurl'),
    dependency('gtk4', version: '>= 4.8.0'),
    dependency('gtkmm-4.0'),
    dependency('libadwaita-1'),
]

# Fetch title list (gtitles.c)
run_command('python', 'scripts/fetch_titles.py', check: true)

subdir('data')
subdir('src')

src += resources
src += cdecrypt_sources
src += sources

executable(
    'WiiUDownloader', 
    src, 
    dependencies: dependencies,
    c_args: c_args,
    cpp_args: cpp_args,
    include_directories: include_directories('include')
)
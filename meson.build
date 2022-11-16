project('WiiUDownloader', ['c', 'cpp'],
    version: '0.1',
    default_options : [ 'c_std=c99', 'cpp_std=c++20'])

compiler = meson.get_compiler('c')
gnome = import('gnome')


c_args = [
    '-D_GNU_SOURCE',    
    '-DAES_ROM_TABLES',    
    '-fno-strict-aliasing',
    '-pipe'
]

cpp_args = [ 
    '-D_GNU_SOURCE',    
    '-DAES_ROM_TABLES',
    '-fno-strict-aliasing',
    '-pipe',
    '-Wall'
]

if get_option('release')
    c_args += ['-fvisibility=hidden', '-Ofast']
    cpp_args += ['-fvisibility=hidden', '-Ofast']
endif

src =  []

dependencies = [
    compiler.find_library('mbedtls'),
    compiler.find_library('mbedcrypto'),
    compiler.find_library('mbedx509'),    
    dependency('libcurl'),
    dependency('gtk4', version: '>= 4.6.0'),
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
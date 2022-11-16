blueprints = custom_target('blueprints',
    input: files(
        'wiiudownloader.blp',
    ),
    output: '.',
    command: [
        find_program('blueprint-compiler'), 
        'batch-compile', 
        '@OUTPUT@', 
        '@CURRENT_SOURCE_DIR@', 
        '@INPUT@'
    ],
)

resources = gnome.compile_resources(
    'wiiudownloader',
    'wiiudownloader.gresource.xml',
    c_name: 'wiiudownloader',
    dependencies: blueprints,
)
blueprints = custom_target('blueprints',
    input: files(
        'main_window.blp',
        
        # Column View Blueprints
        #'column_view_items/index.blp',
        #'column_view_items/queued.blp',
        #'column_view_items/title_id.blp',
        #'column_view_items/kind.blp',
        #'column_view_items/region.blp',
        #'column_view_items/name.blp',
        'column_view.blp',
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
    'resources',
    'gresources.xml',
    c_name: 'resources',
    dependencies: blueprints,
)
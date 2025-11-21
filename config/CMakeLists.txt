# Gather configuration files (INI, XML, TYPES, CFG)
# GLOB_RECURSE finds files in subfolders if any, GLOB is strictly current folder
file(GLOB CONFIG_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ini"   # Critical: auth.ini (Secrets)
    "${CMAKE_CURRENT_SOURCE_DIR}/*.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.types"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cfg"
)

# Create a target that runs "cmake -E copy_if_different"
# This ensures that if you change auth.ini, it updates in the build folder
add_custom_target(DeployConfigs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CONFIG_FILES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Deploying configuration (auth.ini, config.ini) to bin..."
    VERBATIM
)

# Handle Styles (QSS)
file(GLOB_RECURSE STYLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/styles/*.qss")
if(STYLE_FILES)
    add_custom_target(DeployStyles ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/styles
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${STYLE_FILES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/styles
        COMMENT "Deploying styles..."
        VERBATIM
    )
endif()
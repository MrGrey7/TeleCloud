# Gather configuration files
file(GLOB CONFIG_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ini"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.types"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cfg"
)

# Create a target that runs "cmake -E copy_if_different"
add_custom_target(DeployConfigs ALL
    # FIX: Create the directory first!
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    
    # Then copy
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CONFIG_FILES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Deploying configuration to bin..."
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
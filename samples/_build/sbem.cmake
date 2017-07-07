# Generate SBEM code from yaml files
set(GENERATED_SBEM_SOURCES
  "${CMAKE_BINARY_DIR}/generated/sbem-code/sbem_definitions.cpp"
  "${CMAKE_BINARY_DIR}/generated/sbem-code/sbem_definitions.h"
)

set(ALL_YAML_GLOBS ${MOVESENSE_CORE_LIBRARY}/resources/whiteboard/builtinTypes/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/whiteboard/services/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/core/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/meas/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/wbresources/*.yaml)
set(RESOURCE_FILE_FOLDERS ${MOVESENSE_CORE_LIBRARY}/generated/)
file(GLOB ALL_YAML_FILES ${ALL_YAML_GLOBS})

add_custom_command(
    OUTPUT ${GENERATED_SBEM_SOURCES}
    DEPENDS ${ALL_YAML_FILES} ${TOOLS_PATH}/sbemCodeGenerator/sbemCodeGen.py
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/sbem-code"
    COMMENT "Generating SBEM serialization code"
    COMMAND ${PATH_TOOL_PYTHON} ${TOOLS_PATH}/sbemCodeGenerator/sbemCodeGen.py ${ALL_YAML_GLOBS} ${RESOURCE_FILE_FOLDERS}
)

# Add generated sources to the project
set(SOURCES ${SOURCES} 
    ${GENERATED_SBEM_SOURCES}
    )
    
    
# Add generated folder to includes
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated/wb-resources)

# Generate SBEM code from yaml files
set(GENERATED_SBEM_SOURCES
  "${CMAKE_BINARY_DIR}/generated/sbem-code/sbem_definitions.cpp"
  "${CMAKE_BINARY_DIR}/generated/sbem-code/sbem_definitions.h"
)

set(CORE_YAML_GLOBS ${MOVESENSE_CORE_LIBRARY}/resources/whiteboard/builtinTypes/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/whiteboard/services/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/movesense-core/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/*.yaml ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/meas/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/wbresources/*.yaml)
set(RESOURCE_FILE_FOLDERS ${MOVESENSE_CORE_LIBRARY}/generated/${COMPILER}/${CMAKE_BUILD_TYPE})

# Add module API folders to GLOBS
foreach(MODULE_DIR ${MOVESENSE_MODULES})
    set(MODULE_YAML_GLOBS ${MODULE_YAML_GLOBS} ${MODULE_DIR}/wbresources/*.yaml)
endforeach(MODULE_DIR)

foreach(CUSTOM_YAML_PATH ${CUSTOM_YAML_PATHS})
    set(CUSTOM_YAML_GLOBS ${CUSTOM_YAML_GLOBS} ${CMAKE_CURRENT_SOURCE_DIR}/${CUSTOM_YAML_PATH}/*.yaml)
endforeach(CUSTOM_YAML_PATH)

file(GLOB CUST_YAML_FILES ${CUSTOM_YAML_GLOBS})

set(ALL_YAML_GLOBS ${CORE_YAML_GLOBS} ${MODULE_YAML_GLOBS} ${CUSTOM_YAML_GLOBS})
file(GLOB ALL_YAML_FILES ${ALL_YAML_GLOBS})

file(GLOB SBEM_GEN_FILES ${TOOLS_PATH}/sbemCodeGenerator/*.*)

add_custom_command(
    OUTPUT ${GENERATED_SBEM_SOURCES}
    DEPENDS ${ALL_YAML_FILES} ${SBEM_GEN_FILES}
    DEPENDS ${MOVESENSE_CORE_WBO_FILES} ${APP_RESOURCES} ${MODULES_RESOURCES}
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

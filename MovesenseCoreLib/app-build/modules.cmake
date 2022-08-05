# This file imports all listed external modules to the project

foreach(module_dir ${MOVESENSE_MODULES})
    message("Handling module: " ${module_dir})
    
    if(NOT IS_ABSOLUTE ${module_dir})
        set(module_dir ${CMAKE_BINARY_DIR}/${module_dir})
    endif()

    # find all module yaml files and collect them
    file(GLOB YAML_FILES ${module_dir}/wbresources/*.yaml)
    set(MOVESENSE_MODULE_YAML_FILES ${MOVESENSE_MODULE_YAML_FILES} ${YAML_FILES})

    # add module sources
    file(GLOB MOD_SOURCES  ${module_dir}/*.cpp ${module_dir}/*.c)
    set(MODULES_SOURCES ${MODULES_SOURCES} ${MOD_SOURCES})

    # And module folder to includes
    include_directories(${module_dir})

endforeach(module_dir)


# Generate module resources for all modules
if (MOVESENSE_MODULE_YAML_FILES)
    set(MODULES_RESOURCES ${CMAKE_BINARY_DIR}/modules-resources.wbo)
    generate_wb_resources(
        modules-resources MODULES_RESOURCE_SOURCES
        INCLUDE_DIRECTORIES ${WB_DIRECTORY}/include/whiteboard/builtinTypes
        SOURCE_GROUP modules ${MOVESENSE_MODULE_YAML_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/*_root.yaml
        GENERATE CPP LIB
        CPP_DEPENDS wb-resources)
endif()



# This file imports all listed external modules to the project

# Found in https://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# The code that takes pre-built module into use for firmware application
set(module_idx 1)
set(MODULE_LIBRARIES "")
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

    # Add module folder to includes
    include_directories(${module_dir})

    # Add module sub-folders to includes
    SUBDIRLIST(MODULE_SUBDIRS ${module_dir})

    # Remove folders with special meaning
    list(REMOVE_ITEM MODULE_SUBDIRS "wbresources")
    list(REMOVE_ITEM MODULE_SUBDIRS "lib")

    set(ABS_SUBDIRS "")
    foreach(path ${MODULE_SUBDIRS})
        if(NOT IS_ABSOLUTE ${path})
            list(APPEND ABS_SUBDIRS "${module_dir}/${path}")
        else()
            list(APPEND ABS_SUBDIRS "${path}")
        endif()
    endforeach()

    include_directories(${ABS_SUBDIRS})

    # Add pre-built static library
    set(MODULE_LIB_NAME "Module_${module_idx}")
    file(GLOB MOD_LIB  ${module_dir}/lib/${COMPILER}/${CMAKE_BUILD_TYPE}/*.*)
    if (MOD_LIB)
        add_library(${MODULE_LIB_NAME} STATIC IMPORTED)
        SET_PROPERTY(TARGET ${MODULE_LIB_NAME} PROPERTY IMPORTED_LOCATION ${MOD_LIB})
        list(APPEND MODULE_LIBRARIES "${MODULE_LIB_NAME}")
    endif()
    
    set(module_idx ${module_idx}+1)
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



if (IS_MODULE_BUILD)

    ## Code to package 3rd party modules
    if (NOT MODULE_PKG_NAME)
        message("MODULE_PKG_NAME not set. Using ${CMAKE_PROJECT_NAME}.zip")
        set(MODULE_PKG_NAME ${CMAKE_PROJECT_NAME})
    endif()

    set(OUTPUT_MODULE_DIR "${CMAKE_BINARY_DIR}/module_pkg")

    # Find module public sources (the ones that are distributed as source files)
    # These must be in the module root folder
    file(GLOB MODULE_PUBLIC_SOURCES *.c *.cpp *.h *.hpp)

    # find all module source files that are in subfolders of project source dir
    foreach(sub_dir ${MODULE_INTERNAL_FOLDERS})
        message("Handling module_dir: " ${sub_dir})
        file(GLOB SUBDIR_INTERNAL_SOURCES ${sub_dir}/*.c ${sub_dir}/*.cpp)

        set(MODULE_INTERNAL_SOURCES ${MODULE_INTERNAL_SOURCES} ${SUBDIR_INTERNAL_SOURCES})
    endforeach(sub_dir)

    # Copy module internal headers that are to be published as a part of modules
    foreach(sub_dir ${MODULE_INTERNAL_HEADER_FOLDERS})
        file(GLOB_RECURSE SUBDIR_INTERNAL_HEADERS FOLLOW_SYMLINKS
            ${sub_dir}/*.h*)
        file(COPY ${SUBDIR_INTERNAL_HEADERS} DESTINATION ${OUTPUT_MODULE_DIR}/${sub_dir})
    endforeach(sub_dir)

    # create a temporary library for building the internal sources
    set(INTERNAL_LIB_NAME "${CMAKE_PROJECT_NAME}_internal")
    add_library(${INTERNAL_LIB_NAME} STATIC ${MODULE_INTERNAL_SOURCES})

	add_custom_command(
            OUTPUT ${OUTPUT_MODULE_DIR}/lib/${COMPILER}/${CMAKE_BUILD_TYPE}
            DEPENDS ${INTERNAL_LIB_NAME} ${OUTPUT_SUBDIRS}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/wbresources ${OUTPUT_MODULE_DIR}/wbresources
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_MODULE_DIR}/lib/${COMPILER}/${CMAKE_BUILD_TYPE}
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${INTERNAL_LIB_NAME}> ${OUTPUT_MODULE_DIR}/lib/${COMPILER}/${CMAKE_BUILD_TYPE}
            COMMAND ${CMAKE_COMMAND} -E copy ${MODULE_PUBLIC_SOURCES} ${OUTPUT_MODULE_DIR}
			
            COMMENT "Create module package"
    )
    
	add_custom_command(
			OUTPUT ${MODULE_PKG_NAME}
            DEPENDS ${INTERNAL_LIB_NAME} ${OUTPUT_SUBDIRS} ${OUTPUT_MODULE_DIR}/lib/${COMPILER}/${CMAKE_BUILD_TYPE}

#			WORKING_DIRECTORY ${OUTPUT_MODULE_DIR}
#            COMMAND ${CMAKE_COMMAND} -E tar "cfv" "../${MODULE_PKG_NAME}" --format=zip "../${MODULE_PKG_NAME}"
#            COMMENT "Zip the module package"
	)
    
	
	# Create targets for flashing and hex generation
    add_custom_target(pkgmodule DEPENDS ${MODULE_PKG_NAME})
endif()

find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
#get_target_property(${TARGET_NAME} TARGET_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE PUBLIC_HEADERS ${PUBLIC_HEADER_DIR}/*.hpp)
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)
# replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(
OUTPUT ${DOXYGEN_INDEX_FILE}
DEPENDS ${PUBLIC_HEADERS}
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
COMMENT "Generating documentation with Doxygen"
)
add_custom_target(doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
else (DOXYGEN_FOUND)
message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

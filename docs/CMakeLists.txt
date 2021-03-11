option(BUILD_DOC "Build documentation" ON)

set(DOXYFILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
find_package(Doxygen)
if (DOXYGEN_FOUND)
    message("Building Docs")

    add_custom_target(
            docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )

else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the documentation")
endif (DOXYGEN_FOUND)
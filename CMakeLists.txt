# library name
set(LIBNAME "milkimagegen")
set(SRCNAME "image_gen")

message("")
message(" SRCNAME = ${SRCNAME} -> LIBNAME = ${LIBNAME}")

set(SOURCEFILES
	${SRCNAME}.c)

set(INCLUDEFILES
	${SRCNAME}.h)


# DEFAULT SETTINGS
# Do not change unless needed
# =====================================================================

project(lib_${LIBNAME}_project)

# Library can be compiled from multiple source files
# Convention: the main souce file is named <libname>.c
#
add_library(${LIBNAME} SHARED ${SOURCEFILES})
target_include_directories(${LIBNAME} PUBLIC "${PROJECT_SOURCE_DIR}/..")
target_link_libraries(${LIBNAME} PUBLIC CLIcore)

install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${INCLUDEFILES} DESTINATION include/${SRCNAME})

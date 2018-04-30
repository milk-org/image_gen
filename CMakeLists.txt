project(lib_image_gen_project)

include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/..")
add_library(image_gen SHARED image_gen.c)

install(TARGETS image_gen DESTINATION lib)
install(FILES image_gen.h DESTINATION include)


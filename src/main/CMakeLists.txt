aux_source_directory(. SRCS)
add_executable(TMMS ${SRCS})

# add_dependencies(base jsoncpp)
# target_link_libraries(TMMS base jsoncpp_static.a)
install(TARGETS TMMS RUNTIME DESTINATION sbin)
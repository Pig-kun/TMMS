aux_source_directory(. SRCS)
add_executable(TMMS ${SRCS})
install(TARGETS TMMS RUNTIME DESTINATION sbin)
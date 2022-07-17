add_executable(eie_device main.c)

target_link_libraries(eie_device ${LIB_NAME})

install(
  TARGETS eie_device
  RUNTIME DESTINATION bin
)

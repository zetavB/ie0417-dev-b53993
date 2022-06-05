add_executable(eie_device_server main.c)

target_link_libraries(eie_device_server ${LIB_NAME})

install(
  TARGETS eie_device_server
  RUNTIME DESTINATION bin
)

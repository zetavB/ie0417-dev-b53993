add_executable(eie_device_test main.c)

target_link_libraries(eie_device_test ${LIB_NAME} ${CJSON_LIBRARIES})

install(
  TARGETS eie_device
  RUNTIME DESTINATION bin
)

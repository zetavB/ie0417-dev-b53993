# Include rules
include(FindPkgConfig)

pkg_check_modules(CZMQ libczmq)
link_directories(${CZMQ_LIBRARY_DIRS})
include_directories(${CZMQ_INCLUDE_DIRS})

add_executable(eie_device_server main.c)
target_link_libraries(eie_device_server pthread ${CZMQ_LIBRARIES} ${LIB_NAME})

install(
  TARGETS eie_device_server
  RUNTIME DESTINATION bin
)

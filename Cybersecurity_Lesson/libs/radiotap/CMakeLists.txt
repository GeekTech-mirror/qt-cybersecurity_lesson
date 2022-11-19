project(radiotap)

add_definitions("-D_DEFAULT_SOURCE -DRADIOTAP_SUPPORT_OVERRIDES")

add_library(radiotap SHARED radiotap.c)
set_target_properties(radiotap PROPERTIES
		      COMPILE_FLAGS "-Wall -Wextra")

install(TARGETS radiotap DESTINATION lib)
install(FILES radiotap.h radiotap_iter.h DESTINATION include)
target_include_directories(radiotap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(parse parse.c)
set_target_properties(parse PROPERTIES
		      COMPILE_FLAGS "-Wall -Wextra")
target_link_libraries(parse radiotap)


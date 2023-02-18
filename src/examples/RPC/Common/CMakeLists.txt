#add_library(common_net ${net_SRCS})
#target_link_libraries(common_net common_base)

#add_library(common_net_cpp11 ${net_SRCS})
#target_link_libraries(common_net_cpp11 common_base_cpp11)
#set_target_properties(common_net_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

#install(TARGETS common_net DESTINATION lib)
#install(TARGETS common_net_cpp11 DESTINATION lib)
#install(FILES ${HEADERS} DESTINATION include/..)

set(TARGET_NAME net_common)


file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE EDITOR_SOURCES_CC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
file(GLOB_RECURSE EDITOR_SOURCES_CPP CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#file(GLOB_RECURSE EDITOR_RESOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)


add_executable(${TARGET_NAME}  ${EDITOR_HEADERS} ${EDITOR_SOURCES_CPP} ${EDITOR_SOURCES_CC})

include_directories(${PROJECT_SOURCE_DIR})

if(common_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()
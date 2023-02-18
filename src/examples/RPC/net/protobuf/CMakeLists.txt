add_library(common_protobuf_codec ProtobufCodecLite.cc)
set_target_properties(common_protobuf_codec PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(common_protobuf_codec common_net protobuf z)

#add_library(common_protobuf_codec_cpp11 ProtobufCodecLite.cc)
#set_target_properties(common_protobuf_codec_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x -Wno-error=shadow")
#target_link_libraries(common_protobuf_codec_cpp11 common_net_cpp11 protobuf z)


install(TARGETS common_protobuf_codec DESTINATION lib)
#install(TARGETS common_protobuf_codec_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/protobuf)


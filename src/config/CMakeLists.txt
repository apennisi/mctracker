function(config)
  include_directories(${PROJECT_BINARY_DIR}/../src/config/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/confg/include)
    
  file(GLOB_RECURSE CONFIG_SRC "src/config/src/*.cpp")
    
  add_library(config SHARED ${CONFIG_SRC})
  target_link_libraries(config ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES})
  
endfunction()

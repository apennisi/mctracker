function(utils)
  include_directories(${PROJECT_BINARY_DIR}/../src/homography/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/homography/include)

  include_directories(${PROJECT_BINARY_DIR}/../src/utils/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/utils/include)
    
  file(GLOB_RECURSE UTILS_SRC "src/utils/src/*.cpp" "src/homography/src/*.cpp")
    
  add_library(utils SHARED ${UTILS_SRC})
  target_link_libraries(utils ${OpenCV_LIBS})
  
endfunction()

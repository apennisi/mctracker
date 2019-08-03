function(homography) 
  include_directories(${PROJECT_BINARY_DIR}/../src/homography/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/homography/include)
    
  file(GLOB_RECURSE HOMOGRAPHY_SRC "src/homography/src/*.cpp")
    
  add_library(homography SHARED ${HOMOGRAPHY_SRC})
  target_link_libraries(homography ${OpenCV_LIBS} utils)
  
endfunction()

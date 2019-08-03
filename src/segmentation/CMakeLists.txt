function(segmentation) 
  include_directories(${PROJECT_BINARY_DIR}/../src/segmentation/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/segmentation/include)
    
  file(GLOB_RECURSE SEGMENTATION_SRC "src/segmentation/src/*.cpp")
    
  add_library(segmentation SHARED ${SEGMENTATION_SRC})
  target_link_libraries(segmentation ${OpenCV_LIBS})
  
endfunction()

function(tracker) 
  include_directories(${PROJECT_BINARY_DIR}/../src/tracker/include)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/tracker/include)
    
  file(GLOB_RECURSE TRACKER_SRC "src/tracker/src/*.cpp")
    
  add_library(tracker SHARED ${TRACKER_SRC})
  target_link_libraries(tracker ${OpenCV_LIBS} config)
  
endfunction()

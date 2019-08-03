function(object_detection)
    add_library(darknet SHARED IMPORTED)
    set_property(TARGET darknet PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/darknet/libdarknet.so)
    

    include_directories(${PROJECT_BINARY_DIR}/darknet/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/darknet/include)
    include_directories(${PROJECT_BINARY_DIR}/../src/object_detection/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/object_detection/include)

    file(GLOB_RECURSE DETECTOR "src/object_detection/src/*.cpp")
    
    #compiling libraries
    add_library(objectdetector SHARED ${DETECTOR})
    target_link_libraries(objectdetector darknet)
endfunction()

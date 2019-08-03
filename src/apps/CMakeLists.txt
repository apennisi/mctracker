function(applications)
	file(GLOB TRACKER_SRC "src/apps/src/multi_camera_tracker.cpp")
	add_executable( multi_camera_tracker ${TRACKER_SRC})
	target_link_libraries( multi_camera_tracker ${OpenCV_LIBS} objectdetector segmentation config utils homography tracker)
	
	file(GLOB HOMOGRAPHY_SRC "src/apps/src/homography_app.cpp")
	add_executable( homography_app ${HOMOGRAPHY_SRC})
	target_link_libraries( homography_app ${OpenCV_LIBS} homography config)
	
endfunction()

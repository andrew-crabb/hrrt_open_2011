# set (GCC_COMPILE_FLAGS "-std=gnu99")
# add_definitions(${GCC_COMPILE_FLAGS})

add_library (je_hrrt_osem3d 
			file_io_processor.cpp hrrt_osem3d.cpp hrrt_osem3dv_sbrt.cpp
			nr_utils.cpp psf.cpp scanner_model.cpp simd_operation.cpp
			write_image_header.cpp write_ecat_image.cpp interfile_reader.cpp)

target_include_directories (je_hrrt_osem3d PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS je_hrrt_osem3d
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_executable(sogen
    main.cpp
)

target_compile_features(sogen PRIVATE cxx_std_17)

target_link_libraries(sogen PRIVATE grami)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(TARGET sogen PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    target_link_options(sogen PRIVATE -SUBSYSTEM:WINDOWS -ENTRY:mainCRTStartup)
endif()
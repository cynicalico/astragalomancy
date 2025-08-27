add_executable(indev)
target_sources(indev PRIVATE indev.cpp)
target_compile_features(indev PRIVATE cxx_std_23)
target_link_libraries(indev PRIVATE astra::astra)

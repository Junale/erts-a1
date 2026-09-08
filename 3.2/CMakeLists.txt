add_executable(ModuleDouble
    main.cpp
)

target_link_libraries(ModuleDouble PRIVATE
    SystemC::systemc
)
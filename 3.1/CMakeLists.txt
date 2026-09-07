add_executable(ModuleSingle
    main.cpp
)

target_link_libraries(ModuleSingle PRIVATE
    SystemC::systemc
)
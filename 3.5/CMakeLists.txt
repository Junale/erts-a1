
add_executable(MasterSlaveAdapter
    main.cpp
)

target_link_libraries(MasterSlaveAdapter PRIVATE
    SystemC::systemc
)
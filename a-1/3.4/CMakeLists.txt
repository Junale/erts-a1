
add_executable(MasterSlave
    main.cpp
)

target_link_libraries(MasterSlave PRIVATE
    SystemC::systemc
)
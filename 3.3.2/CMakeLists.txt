
add_executable(ProducerTwoConsumers
    main.cpp
)

target_link_libraries(ProducerTwoConsumers PRIVATE
    SystemC::systemc
)
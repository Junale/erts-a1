
add_executable(ProducerConsumer
    main.cpp
)

target_link_libraries(ProducerConsumer PRIVATE
    SystemC::systemc
)
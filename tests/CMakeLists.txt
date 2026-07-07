function(add_barman_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE barman_core Qt5::Test)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_barman_test(tst_product)
add_barman_test(tst_order)
add_barman_test(tst_orderdetails)

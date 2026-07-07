function(add_barman_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE barman_core Qt5::Test)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_barman_test(tst_product)
add_barman_test(tst_money)
add_barman_test(tst_businessday)
add_barman_test(tst_draftorder)
add_barman_test(tst_finalize)
add_barman_test(tst_stock)
add_barman_test(tst_ordermanager)
add_barman_test(tst_productmanager)
add_barman_test(tst_scanners)

# Integration test against real MySQL/MariaDB + schema_v2.sql; skips itself
# unless BARMAN_MYSQL_TEST=1, so the unit-test job stays DB-free.
add_barman_test(tst_mysql_repository)

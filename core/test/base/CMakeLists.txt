add_subdirectory(data)

create_test(array)
create_test(exception)
create_test(exception_helpers)
create_test(executor)
create_test(mtx_reader)
add_dependencies(core_test_base_mtx_reader test_data)
create_test(utils)

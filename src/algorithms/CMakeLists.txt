list(APPEND DIR_NAMES sieve_of_eratosthenes)

foreach(dir_name IN LISTS DIR_NAMES)
  add_subdirectory(${dir_name})
endforeach()

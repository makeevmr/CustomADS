function(create_executable_names_from_dirs DIR_NAMES EXECUTABLE_NAMES)
  set(result "")
  foreach(dir IN LISTS ${DIR_NAMES})
    string(PREPEND dir "test_")
    list(APPEND result "${dir}")
  endforeach()

  # Make variable visible to all subdirectories
  set(${EXECUTABLE_NAMES}
      "${result}"
      CACHE INTERNAL "Executable names list")
endfunction()

set(ALGO_DIR algorithms)
set(DS_DIR data_structures)

add_subdirectory(${ALGO_DIR})
add_subdirectory(${DS_DIR})

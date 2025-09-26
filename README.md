# CustomDS

My implementation of widely used data structures

## Run tests

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=[Release|Debug]
cmake --build . --target ${TARGET_NAME}
${EXECUTABLE_PATH}
```

For example
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target test_segment_tree
./unittests/data_structures/test_segment_tree
```

## Unittest targets

### Algorithms
- `test_sieve_of_eratosthenes`

### Data structures
- `test_segment_tree`

## Executable paths

### Algorithms
- `./unittests/algorithms/test_sieve_of_eratosthenes`

### Data structures
- `./unittests/data_structures/test_segment_tree`

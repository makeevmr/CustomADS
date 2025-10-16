# Knuth–Morris–Pratt algorithm

Function `kmpSubstrSearch(text, substr)` finds all occurences of `substr` in `text`.  
Time: `O(|text| + |substr|)`  
Additional memory: `O(|text|)` 

## Run tests
From `build` directory run:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target test_kmp
./unittests/algorithms/test_kmp
```

## Links
- [cp-algorithms.com](https://cp-algorithms.com/string/prefix-function.html)

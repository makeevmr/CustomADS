# Euclidean algorithm

Function `gcd(a, b)` finds greatest common divisor of `a` and `b`
Time: `O(log(min(a, b)))`  
Additional memory: `O(1)` 

Function `lcm(a, b)` finds least common multiple of `a` and `b`
Time: `O(log(min(a, b)))`  
Additional memory: `O(1)` 

## Run tests
From `build` directory run:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target test_euclidean
./unittests/algorithms/test_euclidean
```

## Links
- [cp-algorithms.com](https://cp-algorithms.com/algebra/euclid-algorithm.html)

# PA-CW1

## Extra requirements

* C++ compiler
* CMake
* Make/Ninja/...
* python3
* taskset [optional]

## Conan package manager

```
  pip3 install conan
```

## Clone and build

```
  git clone https://github.com/AnatoliiShablov/PA-CW1.git && \
  mkdir PA-CW1/build && cd PA-CW1/build && \
  cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release && \
  cmake --build .
```


## Run benchmark

### Params

```
  PA_CW1 array_size max_value
```

### Execute

```
  taskset --cpu-list 0-3 ./PA_CW1 100000000 100000
```
or just
```
  ./PA_CW1 100000000 100000
```
to run without core restrictions

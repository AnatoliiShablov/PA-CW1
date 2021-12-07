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

## Possible output

```

SSort:    | 6471ms | 6106ms | 6018ms | 5984ms | 6114ms | Avg: 6138ms
QSort:    | 6174ms | 5999ms | 5895ms | 5950ms | 5977ms | Avg: 5999ms
PQSort:   | 2018ms | 2301ms | 1988ms | 2130ms | 1981ms | Avg: 2083ms
TBBSort:  | 2352ms | 2222ms | 2094ms | 2043ms | 2136ms | Avg: 2169ms


```

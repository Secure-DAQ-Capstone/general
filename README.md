# NMEA Test 

## Git pull with submodules

```bash
git pull --recurse-submodules
```

This will pull the submodules into your local repo

```bash
git submodule update --init --recursive
```

## Build
In the nmea_test directory:

```
mkdir build && cd build
cmake ..
make
```

To run:
```
cd build
./my_project
```
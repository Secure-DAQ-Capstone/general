# NMEA Test 

## Git pull with submodules

```bash
git pull --recurse-submodules
```

This will pull the submodules into your local repo

```bash
git submodule update --init --recursive
```

List the git branches for each submodules

```bash
git submodule foreach 'git branch'
```



## Packages to install

* Protobuf

```bash
apt install -y protobuf-compiler
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


## SSH key setup

Follow the steps from [GitHub](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=linux).

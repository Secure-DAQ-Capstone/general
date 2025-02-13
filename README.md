# NMEA Test

## Git pull with submodules

```bash
git pull --recurse-submodules
```

This will pull the submodules into your local repo

```bash
git submodule update --init --recursive
```

```bash
git submodule update --recursive
```

List the git branches for each submodule

```bash
git submodule foreach 'git branch'
```

## Dependencies

### Protocol Buffer Compiler

Instructions [here](https://grpc.io/docs/protoc-installation/).

```bash
sudo apt install -y protobuf-compiler
```

## Build

In the nmea_test directory:

```bash
mkdir build && cd build
cmake ..
make
```

To run:

```bash
sudo /sbin/ip link set can0 up type can bitrate 250000
cd build
./nmea_reader
```

To run the sub or pub tests

```bash
# To run a publisher
./udp_test pub 

# To run a subscriber
./upd_test sub
```

## SSH key setup

Follow the steps from [GitHub](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=linux).

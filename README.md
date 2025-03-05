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

### NMEA Dependencies

socketCAN must be installed and working on your system - refer to the adapters users guide.  And make sure to 'start up' the CAN port, example:

```bash
$ sudo /sbin/ip link set can0 up type can bitrate 250000
```

## Key Exchange

To set up the keys for the program, please refer to the Security Code [README](https://github.com/Secure-DAQ-Capstone/daq-code/blob/main/README.md)
## Build

In the nmea_test directory:

```bash
mkdir build && cd build
cmake ..
make
```

To run:
1. Ensure the CAN App is running
```bash
sudo /sbin/ip link set can0 up type can bitrate 250000
```
2. Get the `machine-id`
```bash
cat /etc/machine-id
```
You can also use the following table as a reference for the different machine ids.
### IMPORTANT: Since the Ventana boards share the same machine-id, a number has been added at the end of Machine ID of 1, 3, and 4 to denote them as different

| Machine Name | Machine ID |
|--------------|-----------|
| Tony Computer | 06317250767f47b29c2f8d5e4c14e927 |
| Ventana 1 | 4088253719cb401487ea11178840430d1 |
| Ventana 2 | 4088253719cb401487ea11178840430d |
| Ventana 3 | 4088253719cb401487ea11178840430d3 |
| Ventana 4 | 4088253719cb401487ea11178840430d4 |
| Fares' Laptop | |
| Dave's Laptop |6e824f19507d4a1dac5e15101c0ae908|
| Matt's Laptop | |
| Michael's Laptop | |
| Grace's Laptop | |

3. Run the desired application
```bash
cd build
```
   3.1. To run the nmea reader application (Use the machine ID of the current board or device the application is running on):
    
   ```bash
   ./nmea_reader <Machine ID>
   ```
  
   3.2. To run the relay application (Use the machine ID of the current board or device the application is running on)
  
   ```bash
   ./relay_main <Machine ID> [Reciever/Current IP][Published to IP]
   ```
   
   3.3. To run the Main Application without Database support (Use the machine ID of the current board or device the application is running on):
  
   ```bash
   ./app_main <Machine ID>
   ```
   
   3.4. To run the Main Application with Database support (Use the machine ID of the current board or device the application is running on)**[Needs to be tested]**:
   
   ```bash
   ./mongo_app <Machine ID>
   ```

   3.5. To run the sub or pub tests
   
   ```bash
   # To run a publisher
   ./udp_test pub 
    
   # To run a subscriber
   ./upd_test sub
   ```
  
## SSH key setup

Follow the steps from [GitHub](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent?platform=linux).


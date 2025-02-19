# can_writer

## Requirements
Ideally run in a python venv
```
pip install python-can
```
## Set up can0 interface
```
sudo ip link set can0 down
```

```
sudo ip link set can0 up type can bitrate 250000
```

Need to have loopback on
```
sudo ip link set can0 type can loopback on
```

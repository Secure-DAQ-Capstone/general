import can
import time
import struct

def send_hello_message():
    # Create a bus instance for the 'can0' interface with baud rate 250000
    bus = can.interface.Bus(channel='can0', bustype='socketcan', bitrate=250000)

    latitude = 37.7749  # Example Latitude (in decimal degrees)
    longitude = -122.4194  # Example Longitude (in decimal degrees)
    pgn=129025
    pgn_bytes = pgn.to_bytes(pgn, 'little')
    # pgn_bytes = struct.pack('<f', pgn)

    # NMEA 2000 Position Data Format
    # For simplicity, we're manually structuring the data here. NMEA 2000 uses specific formats.
    # You will need to follow the exact byte layout based on the PGN and its specification.
    # For example, let's encode the latitude and longitude into the CAN message.
    
    # Convert the float values to raw byte data (little-endian)
    latitude_bytes = struct.pack('<f', latitude)  # <f means little-endian float
    longitude_bytes = struct.pack('<f', longitude)  # <f means little-endian float

    # Prepare the full CAN data
    data = latitude_bytes + longitude_bytes
    # Create a message with data "Hello"
    message = can.Message(arbitration_id=0x15F80103, data=data, is_extended_id=True)

    print("Sending 'Hello' message at 250000 baud rate. Press Enter to stop...")

    try:
        # Repeatedly send the message until Enter is pressed
        while True:
            bus.send(message)
            print("Message sent on can0: 'Hello'")
            # Wait for a short period to avoid filling the buffer too quickly
            time.sleep(1)  # Adjust this value as needed (in seconds)

    except KeyboardInterrupt:
        # Stop the loop if Enter is pressed
        print("Message sending stopped.")

if __name__ == '__main__':
    send_hello_message()
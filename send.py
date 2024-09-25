import serial
import time
import xmodem
import io

# Serial port configuration
PORT = 'COM10'
BAUDRATE = 115200
TIMEOUT = 1

def main():
    try:
        with serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT) as ser:
            print(f"Connected to {PORT} at {BAUDRATE} baud")

            def getc(size, timeout=1):
                return ser.read(size) or None

            def putc(data, timeout=1):
                return ser.write(data)

            binary_file = 'out.bin'
            execute_address = 0x80000000

            with open(binary_file, 'rb') as f:
                binary_data = f.read()

            # Create a file-like object from binary_data
            binary_stream = io.BytesIO(binary_data)

            # Send the loadx command and wait for U-Boot to be ready
            ser.write(f"loadx 0x{execute_address:08X}\n".encode())

            # Read until LF is found
            response = b''
            while True:
                char = ser.read(1)
                if char == b'\n':
                    break
                response += char

            print(f"U-Boot response: {response.decode(errors='ignore').strip()}")

            time.sleep(0.5)  # Give U-Boot time to respond

            # Read until LF is found
            response = b''
            while True:
                char = ser.read(1)
                if char == b'\n':
                    break
                response += char

            print(f"U-Boot response: {response.decode(errors='ignore').strip()}")

            time.sleep(0.5)  # Give U-Boot time to respond

            print("Sending binary data to U-Boot using XMODEM...")
            modem = xmodem.XMODEM(getc, putc)
            sent = modem.send(binary_stream)

            if sent:
                print(f"XMODEM transfer complete, {sent} bytes sent.")
                time.sleep(1)

                print(f"Executing binary at address 0x{execute_address:08X}...")
                ser.write(f"go 0x{execute_address:08X}\n".encode())

                # Optional: Wait for and print any response from the device
                time.sleep(1)
                response = ser.read(ser.in_waiting).decode(errors='ignore')
                if response:
                    print("Device response:", response)
            else:
                print("XMODEM transfer failed.")

    except serial.SerialException as e:
        print(f"Error: Unable to open serial port {PORT}. {str(e)}")
    except FileNotFoundError:
        print(f"Error: Binary file '{binary_file}' not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {str(e)}")

if __name__ == "__main__":
    main()

import struct
import socket
import sys


def receive_and_print(local_ip, local_port):
    UDPServerSocket = socket.socket(
        family=socket.AF_INET, type=socket.SOCK_DGRAM)
    UDPServerSocket.bind((local_ip, local_port))
    while True:
        bytes_received, address = UDPServerSocket.recvfrom(52)
        unpacked = struct.unpack("BBBBffffffffffff", bytes_received)
        # print(unpacked)
        accel_valid = unpacked[0]
        gyro_valid = unpacked[1]
        mag_valid = unpacked[2]
        pose_valid = unpacked[3]

        accel = unpacked[4:7]
        gyro = unpacked[7:10]
        mag = unpacked[10:13]
        pose = unpacked[13:16]

        if not accel_valid:
            print("Accel not valid!")
        if not gyro_valid:
            print("Gyro not valid!")
        if not mag_valid:
            print("Mag not valid!")
        if not pose_valid:
            print("Pose not valid!")

        print(
            "accel: {:+7.2f} {:+7.2f} {:+7.2f}   "
            "gyro: {:+7.2f} {:+7.2f} {:+7.2f}    "
            "mag: {:+7.2f} {:+7.2f} {:+7.2f}     "
            "pose: {:+7.2f} {:+7.2f} {:+7.2f}    "
            .format(accel[0], accel[1], accel[2],
                    gyro[0], gyro[1], gyro[2],
                    mag[0], mag[1], mag[2],
                    pose[0], pose[1], pose[2]), end='\r')


if __name__ == "__main__":
    local_ip = sys.argv[1]
    local_port = int(sys.argv[2])
    receive_and_print(local_ip, local_port)

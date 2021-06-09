#ifndef UDP_IMU_STREAM_H
#define UDP_IMU_STREAM_H

#include "RTIMULib.h"
#include "UDPClientSocket.h"

class UDPIMUStream {
public:
  UDPIMUStream(std::string address, uint16_t port)
      : address_{address}, port_{port} {}

  void send(const RTIMU_DATA &sample) {
    auto data = convert(sample);
    auto res = client_.sendto(&data, sizeof(data), address_, port_);
    if (res < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  }

private:
  struct IMUSample {
    struct Vector3f {
      float x;
      float y;
      float z;
    };
    struct Quaternion {
      float scalar;
      float i;
      float j;
      float k;
    };
    bool accel_valid;
    bool gyro_valid;
    bool mag_valid;
    bool pose_valid;
    Vector3f accel;
    Vector3f gyro;
    Vector3f mag;
    Quaternion orientation;
  };

  static IMUSample convert(const RTIMU_DATA &sample) {
    IMUSample result;
    result.accel_valid = sample.accelValid;
    result.gyro_valid = sample.gyroValid;
    result.mag_valid = sample.compassValid;
    result.pose_valid = sample.fusionPoseValid;
    result.accel.x = sample.accel.x();
    result.accel.y = sample.accel.y();
    result.accel.z = sample.accel.z();
    result.gyro.x = sample.gyro.x();
    result.gyro.y = sample.gyro.y();
    result.gyro.z = sample.gyro.z();
    result.mag.x = sample.compass.x();
    result.mag.y = sample.compass.y();
    result.mag.z = sample.compass.z();
    result.orientation.scalar = sample.fusionQPose.scalar();
    result.orientation.i = sample.fusionQPose.x();
    result.orientation.j = sample.fusionQPose.y();
    result.orientation.k = sample.fusionQPose.z();
    return result;
  }
  const std::string address_;
  const uint16_t port_;
  UDPClientSocket client_;
};

#endif // UDP_IMU_STREAM_H

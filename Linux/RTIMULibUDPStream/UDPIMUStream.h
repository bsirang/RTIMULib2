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
    client_.sendto(&data, sizeof(data), address_, port_);
  }

private:
  struct IMUSample {
    struct Vector3f {
      float x;
      float y;
      float z;
    };
    bool accel_valid;
    bool gyro_valid;
    bool mag_valid;
    bool pose_valid;
    Vector3f accel;
    Vector3f gyro;
    Vector3f mag;
    Vector3f pose;
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
    result.pose.x = sample.fusionPose.x();
    result.pose.y = sample.fusionPose.y();
    result.pose.z = sample.fusionPose.z();
    return result;
  }
  const std::string address_;
  const uint16_t port_;
  UDPClientSocket client_;
};

#endif // UDP_IMU_STREAM_H

#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <system_error>

class UDPClientSocket {
public:
  UDPClientSocket(std::string host, uint16_t port)
      : socket_fd_{::socket(AF_INET, SOCK_DGRAM, 0)} {
    if (socket_fd_ < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  }

  ssize_t sendto(const void *buf, size_t len, const std::string &address,
                 uint16_t port) {
    struct sockaddr_in dest_addr {};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = port;
    int addr_res = ::inet_pton(AF_INET, address.c_str(), &dest_addr.sin_addr);
    if (addr_res < 0) {
      throw std::system_error(errno, std::generic_category());
    } else if (addr_res == 0) {
      throw std::invalid_argument("address is not valid!");
    }

    return ::sendto(socket_fd_, buf, len, 0,
                    reinterpret_cast<const struct sockaddr *>(&dest_addr),
                    sizeof dest_addr);
  }

  ~UDPClientSocket() { ::close(socket_fd_); }

private:
  const int socket_fd_;
};

#endif // UDP_SOCKET_H

#ifndef linuxrt_base_hpp
#define linuxrt_base_hpp
#include <vector>
#include <tuple>
#include <string>
#include <memory>
#include <iostream>
namespace linuxrt {
namespace glibc {
namespace bits {
namespace sockaddr_h {}
namespace socket_h {}
} // namespace bits
namespace netinet {
namespace in_h {}
} // namespace netinet
namespace ifaddrs_h {}
} // namespace glibc
} // namespace linuxrt

namespace linuxrt::glibc::bits::sockaddr_h {
//#include <bits/sockaddr.h>
typedef unsigned short int sa_family_t;
/*struct sockaddr {
  sa_family_t sa_family;
  char sa_data[14]; /* Address data.  */
//};*/

} // namespace linuxrt::glibc::bits::sockaddr_h

#include <netinet/in.h>

namespace NI_ {
enum class ENUMS : uint8_t {
  NUMERICHOST = 0b00000001, /* Don't try to look up hostname.  */
  NUMERICSERV = 0b00000010, /* Don't convert port number to name.  */
  NOFQDN      = 0b00000100,      /* Only return nodename portion.  */
  NAMEREQD    = 0b00001000,    /* Don't return numeric addresses.  */
  DGRAM       = 0b00010000,      /* Look up UDP service rather than TCP.  */

};
socklen_t MAXHOST = 1025;
} // namespace NI_

// using namespace linuxrt::glibc::bits::sockaddr_h;
// using namespace linuxrt::glibc::bits::socket_h;


#include <linux/if_link.h> //rtnl_link_stats

namespace linuxrt::ifaddrs {
namespace AF_ { // Address Family
// using namespace linuxrt::glibc::bits::sockaddr_h;
static const std::vector<std::tuple<sa_family_t, std::string>> ENOOM{
    {AF_INET, "IPV4"},
    {AF_INET6, "IPV6"},
    {AF_PACKET, "PACKET"},
    {AF_MPLS, "MPLS"}};
const std::string toString(sa_family_t _family) {
  for (const auto it : ENOOM) {
    if (std::get<0>(it) == _family) {
      return std::get<1>(it);
    };
  };
  return "Unhandled family";
}

}; // namespace AF_
#include <ifaddrs.h>
class address_object {
public:
  address_object() = default;
  std::string name;
  sa_family_t e_address_family;
  std::string host;

private:
  uint ifa_flags; // sockios.h SIOCGIFFLAGS
  std::unique_ptr<sockaddr> *ifa_addr;
  std::unique_ptr<sockaddr> *ifa_netmask;
  rtnl_link_stats statistics;

protected:
};
class address_list {
public:
  address_list();
  ~address_list() { freeifaddrs(SLL_head); }
  std::vector<address_object> addresses;

private:
  ifaddrs *SLL_head;
};

// default constructor
#include <netdb.h>
int wrapped_getnameinfo(const struct sockaddr *__sa, socklen_t __salen,
                        char *__host, socklen_t __hostlen, char *__serv,
                        socklen_t __servlen, int __flags) {
  return getnameinfo(__sa, __salen, __host, __hostlen, __serv, __servlen,
                     __flags);
};

int my_getnameinfo(const struct sockaddr *sa, socklen_t addrlen, char *host,
               socklen_t hostlen, char *serv, socklen_t servlen, int flags)
               {
                return 0;
               };

    address_list::address_list() {
  char host[1025];
  int s;
  // int family;
  // ifaddrs.h
  if (getifaddrs(&SLL_head) == -1) { // ifaddrs.h
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }

  for (struct ifaddrs *ifa = SLL_head; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;
    address_object fresh_address_object;
    fresh_address_object.e_address_family = ifa->ifa_addr->sa_family;
    // family = ifa->ifa_addr->sa_family;

    fresh_address_object.name = ifa->ifa_name;

    std::cout << fresh_address_object.name << "\t"
              << static_cast<int>(fresh_address_object.e_address_family) << "\t"
              << AF_::toString(fresh_address_object.e_address_family)
              << std::endl;
    // printf("%-8s %s (%d)\n", ifa->ifa_name,

    if (fresh_address_object.e_address_family == AF_INET) {
      wrapped_getnameinfo( // This function takes an ifaddrs* and clobbers host
          ifa->ifa_addr,   // sockaddr *
          sizeof(socklen_t),
          host,         // char *
          NI_::MAXHOST, // char_len
          NULL, 0, static_cast<int>(NI_::ENUMS::NUMERICHOST));
      printf("\t\taddress: <%s>\n", host);
      fresh_address_object.host = host;
      

    } else if (fresh_address_object.e_address_family == AF_INET6) {
    } // sizeof(struct sockaddr_in6)

    else if (fresh_address_object.e_address_family == AF_PACKET &&
             ifa->ifa_data != NULL) {
      rtnl_link_stats *stats = static_cast<rtnl_link_stats *>(ifa->ifa_data);

      printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
             "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
             stats->tx_packets, stats->rx_packets, stats->tx_bytes,
             stats->rx_bytes);
    }

    /* Display interface name and family (including symbolic
     form of the latter for the common families). */

    addresses.push_back(fresh_address_object);
  };
}
} // namespace linuxrt::ifaddrs
#endif
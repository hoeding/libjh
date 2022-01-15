#ifndef phy_hpp
#define phy_hpp
#include <filesystem>
#include <vector>
namespace jh {

namespace phy {
  static uint64_t guid = 0;
class interface {
public:
  interface(std::filesystem::path _path) {
    fullpath = _path;
  };
  std::string name() { return fullpath.filename(); };
  bool hasUp() { return true; };      // TODO
  bool hasCarrier() { return true; }; // TODO
  std::filesystem::path fullpath;
};
std::vector<jh::phy::interface> enumerate_interfaces() {
  const std::filesystem::path sysfsdir = "/sys/class/net";
  std::vector<jh::phy::interface> interface_glob;
  for (std::filesystem::path interface_dir :
       std::filesystem::directory_iterator{sysfsdir}) {
    interface_glob.emplace_back(jh::phy::interface{interface_dir.filename()});
  }
  return interface_glob;
};

void enumerate_interface_details(interface *inter){
    // for (auto)

};

} // namespace phy
} // namespace jh
#endif
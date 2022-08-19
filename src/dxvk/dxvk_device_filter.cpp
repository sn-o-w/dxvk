#include "dxvk_device_filter.h"

std::string convertUUID(const uint8_t uuid[VK_UUID_SIZE]) {
  std::ostringstream stream;
  for (unsigned int i = 0; i < VK_UUID_SIZE; i++) {
    stream << static_cast<uint32_t>(uuid[i]);
  }
  return stream.str();
}

namespace dxvk {
  
  DxvkDeviceFilter::DxvkDeviceFilter(DxvkDeviceFilterFlags flags)
  : m_flags(flags) {
    m_matchDeviceName = env::getEnvVar("DXVK_FILTER_DEVICE_NAME");
    m_matchDeviceUUID = env::getEnvVar("DXVK_FILTER_DEVICE_UUID");
    
    if (m_matchDeviceName.size() != 0)
      m_flags.set(DxvkDeviceFilterFlag::MatchDeviceName);
    if (m_matchDeviceUUID.size() != 0)
      m_flags.set(DxvkDeviceFilterFlag::MatchDeviceUUID);
  }
  
  
  DxvkDeviceFilter::~DxvkDeviceFilter() {
    
  }
  
  
  bool DxvkDeviceFilter::testAdapter(const VkPhysicalDeviceProperties& properties) const {
    if (properties.apiVersion < VK_MAKE_VERSION(1, 3, 0)) {
      Logger::warn(str::format("Skipping Vulkan ",
        VK_VERSION_MAJOR(properties.apiVersion), ".",
        VK_VERSION_MINOR(properties.apiVersion), " adapter: ",
        properties.deviceName));
      return false;
    }

    if (m_flags.test(DxvkDeviceFilterFlag::MatchDeviceName)) {
      if (std::string(properties.deviceName).find(m_matchDeviceName) == std::string::npos)
        return false;
    }

    if (m_flags.test(DxvkDeviceFilterFlag::SkipCpuDevices)) {
      if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
        Logger::warn(str::format("Skipping CPU adapter: ", properties.deviceName));
        return false;
      }
    }

    return true;
  }

  bool DxvkDeviceFilter::testCreatedAdapter(const DxvkDeviceInfo& deviceInfo) const {
    if (m_flags.test(DxvkDeviceFilterFlag::MatchDeviceUUID)) {
      if (convertUUID(deviceInfo.vk11.deviceUUID).find(m_matchDeviceUUID) == std::string::npos)
        return false;
    }

    return true;
  }
  
}

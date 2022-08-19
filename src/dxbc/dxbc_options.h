#pragma once

#include "../dxvk/dxvk_device.h"

namespace dxvk {

  struct D3D11Options;

  enum class DxbcFloatControlFlag : uint32_t {
    DenormFlushToZero32,
    DenormPreserve64,
    PreserveNan32,
    PreserveNan64,
  };

  using DxbcFloatControlFlags = Flags<DxbcFloatControlFlag>;

  struct DxbcOptions {
    DxbcOptions();
    DxbcOptions(const Rc<DxvkDevice>& device, const D3D11Options& options);

    // Clamp oDepth in fragment shaders if the depth
    // clip device feature is not supported
    bool useDepthClipWorkaround = false;

    /// Determines whether format qualifiers
    /// on typed UAV loads are required
    bool supportsTypedUavLoadR32 = false;
    bool supportsTypedUavLoadExtended = false;

    /// Use subgroup operations to reduce the number of
    /// atomic operations for append/consume buffers.
    bool useSubgroupOpsForAtomicCounters = false;

    /// Enables NaN fixup for render target outputs
    bool enableRtOutputNanFixup = false;

    /// Clear thread-group shared memory to zero
    bool zeroInitWorkgroupMemory = false;

    /// Declare vertex positions as invariant
    bool invariantPosition = false;

    /// Insert memory barriers after TGSM stoes
    bool forceTgsmBarriers = false;

    /// Replace ld_ms with ld
    bool disableMsaa = false;

    /// Float control flags
    DxbcFloatControlFlags floatControl;

    /// Minimum storage buffer alignment
    VkDeviceSize minSsboAlignment = 0;
  };
  
}
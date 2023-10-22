#ifndef CHIMERA_COMPUTE_COMMON_H
#define CHIMERA_COMPUTE_COMMON_H

#include "volk.h"
#include "vk_utils.h"
#include <cstring>
//#include <memory>


struct pipeline_data_t
{
  VkPipelineLayout layout;
  VkPipeline pipeline;
};


class ICompute
{
public:
  virtual VkInstance   GetVkInstance() const = 0;

  virtual void InitVulkan(const char** a_instanceExtensions, uint32_t a_instanceExtensionsCount, uint32_t a_deviceId) = 0;
  virtual void Prepare(unsigned int workgroup_count) = 0;
  virtual void Run() = 0;

  virtual ~ICompute() = default;
};

#endif//CHIMERA_COMPUTE_COMMON_H

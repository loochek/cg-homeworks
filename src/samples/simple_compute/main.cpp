#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>

#include "simple_compute.h"

constexpr int KERNEL_SIZE = 7;
constexpr int WORKGROUP_SIZE = 512;

template<typename Func>
void measureRunTime(Func func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time: " << static_cast<float>(duration.count()) / 1000 << " seconds\n";
}

float computeOnCpu(const std::vector<float> &source)
{
  float result = 0.0f;
  for (int idx = 0; idx < source.size(); idx++) {
    float local_result = 0.0f;
    for (int offset = -KERNEL_SIZE / 2; offset <= KERNEL_SIZE / 2; offset++) {
      int i = idx + offset;
      if (i < 0 || i >= source.size()) {
        continue;
      }

      local_result += source[idx + offset];
    }

    local_result /= KERNEL_SIZE;
    result += source[idx] - local_result;
  }

  return result;
}

int main()
{
  constexpr int LENGTH = 100000000;
  constexpr int VULKAN_DEVICE_ID = 0;

  constexpr int WORKGROUP_COUNT = LENGTH % WORKGROUP_SIZE == 0 ?  LENGTH / WORKGROUP_SIZE : LENGTH / WORKGROUP_SIZE + 1;

  std::shared_ptr<SimpleCompute> app = std::make_unique<SimpleCompute>(LENGTH);
  if(app == nullptr)
  {
    std::cout << "Can't create render of specified type" << std::endl;
    return 1;
  }

  app->InitVulkan(nullptr, 0, VULKAN_DEVICE_ID);
  app->Prepare(WORKGROUP_COUNT);

  std::default_random_engine rand_number(time(nullptr));
  std::uniform_real_distribution<float> distribution(-5.0f,5.0f);

  std::vector<float> values(LENGTH);
  for (int i = 0; i < LENGTH; i++)
  {
    values[i] = distribution(rand_number);
  }

  app->getBufferCopyHelper()->UpdateBuffer(app->getSourceBuffer(), 0, values.data(), sizeof(float) * values.size());

  measureRunTime([&]
  {
    std::cout << "CPU result: " << computeOnCpu(values) << '\n';
  });

  measureRunTime([&]
  {
    app->Run();
    std::vector<float> gpu_result_vector(WORKGROUP_COUNT);
    app->getBufferCopyHelper()->ReadBuffer(app->getResultBuffer(), 0, gpu_result_vector.data(), sizeof(float) * gpu_result_vector.size());
    float gpu_result = 0.0f;
    for (float partial_sum : gpu_result_vector) {
      gpu_result += partial_sum;
    }
    std::cout << "GPU result: " << gpu_result << '\n';
  });

  return 0;
}

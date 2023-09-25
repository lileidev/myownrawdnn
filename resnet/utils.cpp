#include <cstddef>
#include <functional>
#include <vector>
#include <numeric>

#include "utils.h"
#include <bits/stdint-intn.h>

size_t get_tensor_nelem(const std::shared_ptr<Tensor> tensor) {
  int32_t ndim = tensor->ndim;
  std::vector<int64_t> shape(tensor->shape, tensor->shape+ndim);

  return std::accumulate(tensor->shape, tensor->shape+ndim, 1, std::multiplies<size_t>());
}

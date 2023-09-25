#pragma once

#include <cstddef>
#include <memory>

#include "tensor.h"


size_t get_tensor_nelem(const std::shared_ptr<Tensor> tensor);
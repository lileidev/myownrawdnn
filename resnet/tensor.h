#pragma once

#include <bits/stdint-intn.h>
#include <cstdint>

enum DataType {
  INT,
  LONG,
  FLOAT,
  DOUBLE
};

enum DeviceType {
  CPU,
  GPU
};

struct Tensor {
  void *data;

  int32_t ndim;
  int64_t *shape;
  int64_t *strides;

  DataType dtype;

  DeviceType device;

  ~Tensor() {
    if (shape != nullptr) {
      delete [] shape;
    }
    if (strides != nullptr) {
      delete [] strides;
    }
    
    switch (dtype) {
      case DataType::DOUBLE: {
        delete [] static_cast<double*>(data);
        break;
      }
      case DataType::FLOAT: {
        delete [] static_cast<float*>(data);
        break;
      }
      case DataType::INT: {
        delete [] static_cast<int32_t*>(data);
        break;
      }
      case DataType::LONG: {
        delete [] static_cast<int64_t*>(data);
        break;
      }
      default: {
        break;
      }
    }
  }
};

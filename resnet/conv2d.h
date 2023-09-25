
#pragma once 

#include <bits/stdint-intn.h>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <cmath>

#include "tensor.h"
#include "utils.h"


// padding: -1 -> same padding, 0 -> no padding, other positive nunber -> specified padding
template <typename T>
std::shared_ptr<Tensor> conv2d(const std::shared_ptr<Tensor> input, const std::shared_ptr<Tensor> kernel, const int stride, const int padding) {
  assert(kernel->shape[kernel->ndim-1] == kernel->shape[kernel->ndim-2]);
  assert(input->shape[input->ndim-1] == input->shape[input->ndim-2]);
  int kernel_size = kernel->shape[kernel->ndim-1];
  int input_size = input->shape[input->ndim-1];
  
  int padding_value;
  if (padding == -1) {  // same padding
    if (stride == 1) {
      // p = (f - 1) / 2
      padding_value = (kernel_size - 1) / 2;
    } else {
      padding_value = (stride * (input_size - 1) + kernel_size - input_size) / 2;
    }
  } else if (padding == 0) {
    padding_value = 0;
  } else {
    padding_value = padding;
  }
  int padded_size = input_size + 2 * padding_value;

  // calculate output tensor info
  int output_size = (input_size - kernel_size) / stride + 1;
  // create output Tensor
  std::shared_ptr<Tensor> output = std::make_shared<Tensor>();
  // allocate memory and initialize to 0
  output->ndim = input->ndim;
  output->dtype = input->dtype;
  output->device = DeviceType::CPU;
  output->shape = new int64_t[output->ndim];
  output->strides = new int64_t[output->ndim];
  output->shape[output->ndim-1] = output_size;
  output->shape[output->ndim-2] = output_size;
  output->shape[output->ndim-3] = input->shape[input->ndim-3];
  assert(typeid(T) == typeid(double) || typeid(T) == typeid(float) || typeid(T) == typeid(int32_t) || typeid(T) == typeid(int64_t));
  output->data = new T[get_tensor_nelem(output)]();
  
  float* input_data = static_cast<float*>(input->data);
  float* kernel_data = static_cast<float*>(kernel->data);
  float* output_data = static_cast<float*>(output->data);

  int valid_start_index = padding_value;                    // 1
  int valid_end_index = padded_size - padding_value - 1;    // 28 - 1 - 1 = 26
  // convolution implementation
  int output_offset = 0;
  for (int channel = 0; channel < input->shape[input->ndim-3]; channel++) {
    for (int outter_i = 0; outter_i <= padded_size - kernel_size; outter_i += stride) {
      int start_row_index = outter_i;
      for (int outter_j = 0; outter_j <= padded_size - kernel_size; outter_j += stride) {
        int start_col_index = outter_j;
        T sum = 0;
        for (int inner_i = start_row_index; inner_i < start_row_index + kernel_size; inner_i++) {
          if (inner_i < valid_start_index || inner_i > valid_end_index) continue;
          int input_i = inner_i - padding_value;
          int kernel_i = inner_i - start_row_index;
          for (int inner_j = start_col_index; inner_j < start_col_index + kernel_size; inner_j++) {
            if (inner_j < valid_start_index || inner_j > valid_end_index) continue;
            int input_j = inner_j - padding_value;
            int kernel_j = inner_j - start_col_index;
            // index of input data pointer
            int input_index = channel * std::pow(input_size, 2) + input_i * input_size + input_j;
            // index of kernel data pointer
            int kernel_index = channel * std::pow(kernel_size, 2) + kernel_i * kernel_size + kernel_j;
            sum += input_data[input_index] * kernel_data[kernel_index];
          }
        }
        output_data[output_offset] = sum;
        output_offset++;
      }
    }
  }

  return output;
}

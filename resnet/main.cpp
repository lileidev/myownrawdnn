#include <cstring>
#include <iostream>
#include <fstream>

#include "tensor.h"
#include "conv2d.h"
#include "utils.h"
#include "imread.h"
#include <bits/stdint-intn.h>

#define MAX_IMAGE_SIZE 256
#define MAX_IMAGE_BYTES MAX_IMAGE_SIZE * MAX_IMAGE_SIZE * 3

int main(int argc, char **argv) {
  std::ifstream ifs("./test.jpg", std::ifstream::in | std::ios::binary);
  ifs.seekg(0, ifs.end);
  int length = ifs.tellg();
  ifs.seekg(0, ifs.beg);

  char *buffer = new char[length];
  ifs.read(buffer, length);
  std::shared_ptr<Tensor> input = std::make_shared<Tensor>();
  unsigned char* rgb_data = new unsigned char[MAX_IMAGE_BYTES];
  JPeg jpeg_info = read_jpeg_file(buffer, rgb_data, length);

  input->device = DeviceType::CPU;
  input->dtype = DataType::FLOAT;
  input->ndim = 3;
  input->shape = new int64_t[3];
  input->shape[0] = jpeg_info.channels;
  input->shape[1] = jpeg_info.width;
  input->shape[2] = jpeg_info.height;
  int input_nelem = get_tensor_nelem(input);
  input->data = new float[input_nelem];
  for (int i = 0; i < input_nelem; i++) {
    static_cast<float*>(input->data)[i] = static_cast<float>(rgb_data[i]) / 255;
  }

  std::shared_ptr<Tensor> kernel = std::make_shared<Tensor>();
  kernel->device = DeviceType::CPU;
  kernel->dtype = DataType::FLOAT;
  kernel->ndim = 3;
  kernel->shape = new int64_t[3];
  kernel->shape[0] = 3;
  kernel->shape[1] = 3;
  kernel->shape[2] = 3;
  size_t kernel_nelem = get_tensor_nelem(kernel);
  kernel->data = new float[kernel_nelem];
  for (int i = 0; i < kernel_nelem; i++) {
    static_cast<float*>(kernel->data)[i] = static_cast<float>(i) / 10.0;
  }

  std::shared_ptr<Tensor> output = conv2d<float>(input, kernel, 1, 0);

  int output_nelem = get_tensor_nelem(output);
  // rgb_data = new unsigned char[output_nelem];
  // for (int i = 0; i < output_nelem; i++) {
  //   rgb_data[i] = static_cast<float*>(output->data)[i];
  // }

  write_jpeg_file(rgb_data, "1.jpg");

  delete [] rgb_data;

  return 0;
}

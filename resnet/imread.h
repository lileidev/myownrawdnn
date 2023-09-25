#pragma once

#include <bits/types/FILE.h>
#include <jpeglib.h>

struct JPeg {
  unsigned int height;
  unsigned int width;
  unsigned int channels;
};

JPeg read_jpeg_file(const char* jpeg_data, unsigned char *rgb_data, int size);

bool write_jpeg_file(unsigned char* rgb_data, const char* file_name);
#pragma once

#include <bits/types/FILE.h>
#include <jpeglib.h>

struct JPeg {
  unsigned int height;
  unsigned int width;
  unsigned int channels;
};

JPeg read_jpeg_file(const char* jpeg_data, char *rgb_data, int size);

#pragma once

#include <bits/types/FILE.h>
#include <jpeglib.h>

struct ImageInfo {
  unsigned int height;
  unsigned int width;
  unsigned int channels;
};

ImageInfo read_jpeg_file(const char* jpeg_data, unsigned char *rgb_data, int size);

bool write_jpeg_file(unsigned char* rgb_data, const ImageInfo &image_info, const char* file_name);

void convert_rgb2plane(unsigned char* rgb_data, unsigned char* plane_data, const ImageInfo &image_info);

void convert_plane2rgb(unsigned char* plane_data, unsigned char* rgb_data, const ImageInfo &image_info);
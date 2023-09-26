#include "stdlib.h"
#include "string.h"
#include "image.h"

#include <iostream>
#include <cstdio>
#include <jpeglib.h>

ImageInfo read_jpeg_file(const char* jpeg_data, unsigned char *rgb_data, int size) {
  struct jpeg_error_mgr jerr;
  struct jpeg_decompress_struct cinfo;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo, (unsigned char*) jpeg_data, size);
  // get jpeg image parameters stored in header
  jpeg_read_header(&cinfo, true);
  jpeg_start_decompress(&cinfo);
  int row_stride = cinfo.image_width * cinfo.num_components;
  unsigned char* buffer = new unsigned char[row_stride];

  int i = 0; 
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, &buffer, 1);
    memcpy(rgb_data + row_stride * i,  buffer, row_stride);
    i++;
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  delete [] buffer;

  ImageInfo jpeg_info;
  jpeg_info.height = cinfo.image_height;
  jpeg_info.width = cinfo.image_width;
  jpeg_info.channels = cinfo.num_components;

  return jpeg_info;
}

bool write_jpeg_file(unsigned char* rgb_data, const ImageInfo &image_info, const char* file_name) {
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_compress(&cinfo);

  FILE *pFile = fopen(file_name, "wb");
  if (!pFile) {
    std::cout << "Open file error" << std::endl;
    return -1;
  }

  jpeg_stdio_dest(&cinfo, pFile);
  cinfo.image_height = image_info.height;
  cinfo.image_width = image_info.width;
  cinfo.input_components = image_info.channels;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);

  jpeg_start_compress(&cinfo, true);

  JSAMPROW row_pointer[1];

  int row_stride = cinfo.image_width * cinfo.input_components;
  int i = 0; 
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &(rgb_data[cinfo.next_scanline * row_stride]);
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  fclose(pFile);
  pFile = nullptr;

  return 0;
}

void convert_rgb2plane(unsigned char* rgb_data, unsigned char* plane_data, const ImageInfo &image_info) {
  int nelem_of_one_channel = image_info.height * image_info.width;
  int nelem_of_image = nelem_of_one_channel * image_info.channels;
  
  for (int i = 0; i < nelem_of_image; i++) {
    int index = (i / 3) + (i % 3) * nelem_of_one_channel;
    *(plane_data + index) = *(rgb_data + i);
  }
}

void convert_plane2rgb(unsigned char* plane_data, unsigned char* rgb_data, const ImageInfo &image_info) {
  int nelem_of_one_channel = image_info.height * image_info.width;
  int nelem_of_image = nelem_of_one_channel * image_info.channels;

  for (int i = 0; i < nelem_of_image; i++) {
    int index = i % nelem_of_one_channel * 3 + i / nelem_of_one_channel;\
    *(rgb_data + index) = *(plane_data + i);
  }
  
}

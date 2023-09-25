#include "stdlib.h"
#include "string.h"
#include "imread.h"

#include <iostream>
#include <cstdio>
#include <jpeglib.h>

JPeg read_jpeg_file(const char* jpeg_data, unsigned char *rgb_data, int size) {
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

  JPeg jpeg_info;
  jpeg_info.height = cinfo.image_height;
  jpeg_info.width = cinfo.image_width;
  jpeg_info.channels = cinfo.num_components;

  return jpeg_info;
}

bool write_jpeg_file(unsigned char* rgb_data, const char* file_name) {
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
  cinfo.image_height = 240;
  cinfo.image_width = 240;
  cinfo.input_components = 3;
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
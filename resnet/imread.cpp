#include "stdlib.h"
#include "string.h"
#include "imread.h"
#include <jpeglib.h>

JPeg read_jpeg_file(const char* jpeg_data, char *rgb_data, int size) {
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

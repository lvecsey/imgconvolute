
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pixel.h>
#include <image.h>

double kernel[9] = {
  -1.0, -1.0, -1.0
  ,-1.0, 8.0, -1.0
  -1.0, -1.0, -1.0
};

int main(int argc, char *argv[]) {

  long int xres = 614, yres = 768;
  long int num_pixels = xres * yres;
  size_t img_sz = sizeof(pixel_t) * num_pixels;
  
  long int xw = xres - 2, yh = yres - 2;
  long int xi, yi;


  
  image_t img = { .rgb = malloc(img_sz), .xres = xres, .yres = yres };

  unsigned char *out1 = malloc(xw*yh);
  unsigned char *out2 = malloc(xw*yh);
  unsigned char *out3 = malloc(xw*yh);  
  
  long int n;

  for (yi = 0; yi < yh; yi++) {
    for (xi = 0; xi < xw; xi++) {

    }
  }

  {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd;

    fd = open("out1.gray", O_CREAT | O_APPEND | O_WRONLY, mode);
    write(fd, out1, xw*yh);

    fd = open("out2.gray", O_CREAT | O_APPEND | O_WRONLY, mode);
    write(fd, out2, xw*yh);
    
    fd = open("out3.gray", O_CREAT | O_APPEND | O_WRONLY, mode);
    write(fd, out3, xw*yh);    

  }
  
  return 0;

}

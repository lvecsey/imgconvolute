
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pixel.h>
#include <image.h>

#include <sys/mman.h>

double kernel[9] = {
  -1.0, -1.0, -1.0
  ,-1.0, 8.0, -1.0
  -1.0, -1.0, -1.0
};

enum { PIXEL_R, PIXEL_G, PIXEL_B };

unsigned char getpixel(long int xpos, long int ypos, pixel_t *rgb, long int xres, long int pix, double *k) {

  long int j,i;

  long int sum = 0;
  
  for (j = 0; j < 3; j++) {
    for (i = 0; i < 3; i++) {

      switch(pix) {
      case PIXEL_R:
	sum += k[j*3+i] * (rgb[ (ypos+j) * xres + i ].r >> 8);
	break;
      case PIXEL_G:
	sum += k[j*3+i] * (rgb[ (ypos+j) * xres + i ].g >> 8);
	break;
      case PIXEL_B:
	sum += k[j*3+i] * (rgb[ (ypos+j) * xres + i ].b >> 8);
	break;
      }
      
    }
  }

  sum /= 9;
  
  return sum;
  
}

int main(int argc, char *argv[]) {

  long int xres = 614, yres = 768;
  long int num_pixels = xres * yres;
  size_t img_sz = sizeof(pixel_t) * num_pixels;
  
  long int xw = xres - 2, yh = yres - 2;
  long int xi, yi;
  
  image_t img = { .rgb = NULL, .xres = xres, .yres = yres };

  unsigned char *out1 = malloc(xw*yh);
  unsigned char *out2 = malloc(xw*yh);
  unsigned char *out3 = malloc(xw*yh);  
  
  long int n;

  char *filename = argc>1 > argv[1] : NULL;

  struct stat buf;
  
  int fd = open(filename, O_RDWR);
  fstat(fd, &buf);
  img.rgb = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (img.rgb == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  
  for (yi = 0; yi < yh; yi++) {
    for (xi = 0; xi < xw; xi++) {

      out1[yi*xw+xi] = getpixel(yi+1, xi+1, img.rgb, xres, PIXEL_R, kernel);
      out2[yi*xw+xi] = getpixel(yi+1, xi+1, img.rgb, xres, PIXEL_G, kernel);
      out3[yi*xw+xi] = getpixel(yi+1, xi+1, img.rgb, xres, PIXEL_B, kernel);          
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define TYPE 3
#define WIDTH 750
#define HEIGHT 750
#define SCALE 255

int main() {  
  int fd = open("pic.ppm", O_CREAT | O_RDWR | O_APPEND | O_TRUNC, 0644);

  char header[32];
  
  int len = 0;
  len = sprintf(header, "P%d\n", TYPE);
  len += sprintf(header + len, "%d %d\n", HEIGHT, WIDTH);
  sprintf(header + len, "%d\n", SCALE);

  write(fd, header, strlen(header));

  unsigned char r, g, b;
  
  int i;
  for(i=0; i<HEIGHT; i++){
    char pixel[16];

    int j;
    for(j=0; j<WIDTH; j++){
      r = (i+j) % (i+1);
      g = (2*i+j) % (j+1);
      b = (i+2*j) % (i+j+1);
      
      sprintf(pixel, "%d %d %d", r, g, b);
      
      if(j != WIDTH-1) {
	strcat(pixel, " ");
	write(fd, pixel, strlen(pixel));
      } else {	
	write(fd, pixel, strlen(pixel));
      }
    }
    
    write(fd, "\n", 1);
  }
  
  close(fd);

  return 0;
}

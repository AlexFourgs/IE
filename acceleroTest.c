#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include 

int main(){
int file;
char *acceleroPath="/dev/i2c-2";
char valueAcc;

while (1){
  i2cget(0x2D);
  printf("%s",valueAcc);
}


  return 0;
}

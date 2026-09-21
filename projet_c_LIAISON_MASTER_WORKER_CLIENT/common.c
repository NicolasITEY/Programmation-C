
#include "myassert.h"
#include <unistd.h>
#include "common.h"

void myPipe(int *pipeArr){
    int ret = pipe(pipeArr);
	myassert(ret == 0, "creation de pipe rate");
}

void myClose(int fd){
    int ret = close(fd);
    myassert(ret != -1,"la fermeture n'a pas été fait correctement");
}

void myRead(int fd,void *to,long unsigned int bytes){
    long unsigned int res =read(fd,to,bytes);
	myassert(res == bytes, "read raté");
}
void myWrite(int fd,void *from,long unsigned int bytes){
    long unsigned int res =write(fd,from,bytes);
	myassert(res == bytes, "write raté");
}


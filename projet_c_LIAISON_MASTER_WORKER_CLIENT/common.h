#ifndef COMMON_H
#define COMMON_H

void myRead(int fd,void *to,long unsigned int bytes);
void myWrite(int fd,void *from,long unsigned int bytes);
void myPipe(int *pipeArr);
void myClose(int fd);

#endif

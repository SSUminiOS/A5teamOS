#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<unistd.h>
#include<stddef.h>
#include<sys/types.h>
#include<sys/mman.h>

#define SHM_SIZE 1048576


void SHM(){

int shm_fd;
void *ptr;

char mem[SHM_SIZE]={0};

shm_fd = shm_open("memory", O_CREAT | O_RDWR, 0666);
if(shm_fd==-1){
perror("shm_open");
exit( EXIT_FAILURE);}

ftruncate(shm_fd, SHM_SIZE);

ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd,0);

if(ptr== MAP_FAILED){
perror("mmap");
exit( EXIT_FAILURE);}

int *memindex=((int*)ptr);
*memindex=(int)8;
char *size = ((char*)ptr+4);
char *shmallocflag=((char*)ptr+5);
char *shmfreeflag=((char*)ptr+6);
*size=0;
*shmallocflag=0;
*shmfreeflag=0;

while(1){
if(*shmallocflag==1){


while(mem[*memindex+*size-1])
{*memindex+=*size;}

mem[*memindex-8]=1;

*memindex+=*size;
*shmallocflag=(char)0; 
}

else if(*((char*)ptr+6)==1){

mem[*memindex-8]=0;
 
*shmfreeflag=(char)0; 
}
usleep(1000);
if(getppid()==1){

munmap(ptr,SHM_SIZE);
shm_unlink("memory");

exit(0);};

}}


void *shmalloc(char size){
int shm_fd;
void *temp;
void *ptr;
shm_fd = shm_open("memory", O_CREAT | O_RDWR, 0666);
if(shm_fd==-1){
perror("shm_open");
exit( EXIT_FAILURE);}

ptr=mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd,0);
if(ptr== MAP_FAILED){
perror("mmap");
exit( EXIT_FAILURE);}


*((char*)ptr+4)=(char)size;
*((char*)ptr+5)=(char)1;
while(*((char*)ptr+5)){}

temp=(void*)((char*)ptr+*((int*)ptr));
printf("%d",*((int*)ptr));
return temp;
}

void shfree(void *adress){
int shm_fd;
void *ptr;
shm_fd = shm_open("memory", O_CREAT | O_RDWR, 0666);
if(shm_fd==-1){
perror("shm_open");
exit( EXIT_FAILURE);}
ptr=mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd,0);
if(ptr== MAP_FAILED){
perror("mmap");
exit( EXIT_FAILURE);}


ptrdiff_t temp=(char*)adress-(char*)ptr;
*((char*)ptr)=(int)temp;
*((char*)ptr+6)=(char)1;
while(*((char*)ptr+6)){}
}


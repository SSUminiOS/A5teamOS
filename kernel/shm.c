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

char mem[SHM_SIZE]={0}; //shm을 할당하는데 있어서 해당 메모리가 할당되어있는지 아닌지 체크하는 배열

shm_fd = shm_open("memory", O_CREAT | O_RDWR, 0666); //shm 세그먼트생성
if(shm_fd==-1){
perror("shm_open");
exit( EXIT_FAILURE);}

ftruncate(shm_fd, SHM_SIZE); 

ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd,0); 

if(ptr== MAP_FAILED){
perror("mmap");
exit( EXIT_FAILURE);}

int *memindex=((int*)ptr); //shm의 앞의 8비트는 IPC를 하기위해 할당
*memindex=(int)8;//배정할 shm의 index를 첫 주소에 저장
char *size = ((char*)ptr+4); // shmalloc 함수로 배정할 메모리의 크기
char *shmallocflag=((char*)ptr+5);//shmalloc 함수가 실행되면 해당 주소의 값 1로 설정한다.
char *shmfreeflag=((char*)ptr+6);//shfree함수가 실행되면 해당 주소의 값을 1로 설정한다.
*size=0;
*shmallocflag=0;
*shmfreeflag=0;

while(1){
if(*shmallocflag==1){//shmalloc 함수 실행됌


while(mem[*memindex+*size-1])//mem[*memindex+*size-1]의 값이 1이면 메모리가 할당되어있다는 뜻이다.
{*memindex+=*size;} //비어있을때까지 할당할 size만큼 증가시킨다.

mem[*memindex-8]=1;//할당가능하면 할당한다.

*memindex+=*size; //index를 size만큼 키워서 다음 할당을 준비
*shmallocflag=(char)0;//flag를 0으로 설정하여 shmalloc 함수에서 다음단계 실행
}

else if(*((char*)ptr+6)==1){//shfree함수 실행됌

mem[*memindex-8]=0;//할당해제
 
*shmfreeflag=(char)0; 
}
usleep(1000);
if(getppid()==1){//부모프로세스가 운영체제가되면 shm할당해제하고 해당프로세스 종료

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
exit( EXIT_FAILURE);}//shm에 접근하여 첫 주소 받음


*((char*)ptr+4)=(char)size;//4번째 바이트에 할당할 크기 설정
*((char*)ptr+5)=(char)1;//flag를 1로설정하여 SHM()함수에서 처리
while(*((char*)ptr+5)){}//flag가0이될때까지 대기

temp=(void*)((char*)ptr+*((int*)ptr));//할당할 주소 계산
return temp;//주소할당
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


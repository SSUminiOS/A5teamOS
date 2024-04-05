#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
void minisystem()
{
    printf("minisystem\n");
}



typedef struct dir{ //폴더구조 모방
char name[10];
struct dir* subdirs[5];
struct dir* parentdir;
int num_of_subdir;
}dir;

dir* currentdir;//현재 폴더

void minimkroot(){ //root 폴더 생성 minios가 실행될때 호출된다.
dir* root = shmalloc(sizeof(dir));
strcpy(root->name, "root");
root->subdirs[0]=NULL;
root->parentdir=root;
root->num_of_subdir=0;
currentdir=root;

printf("%ld", sizeof(dir));
}


void minicd(char *dir_name){//폴더 이동
int i=0;


if(strcmp(dir_name,"..")==0){
currentdir=currentdir->parentdir;
printf("Current diractory is %s\n",currentdir->name);
}


else{
if(currentdir->subdirs[0]==NULL)
{printf("Directory not found\n");
return;}


while(i<currentdir->num_of_subdir&&!(strcmp(currentdir->subdirs[i]->name,dir_name)==0)){
i++;}
if(i==currentdir->num_of_subdir)
printf("Directory not found\n");
else {currentdir=currentdir->subdirs[i];
printf("Current diractory is %s\n",currentdir->name);}
}
}

void minils(){//현재 폴더의 subdir 출력
int i;
for (i=0;i<currentdir->num_of_subdir;i++){
	printf("%s ",currentdir->subdirs[i]->name);}
	
	printf("\n");
}

void minimkdir(const char *dir_name){//폴더생성 shmalloc 함수를 사용하여 shm내에 주소값을 할당받는다
const char *name=dir_name;

dir* subdir = shmalloc(sizeof(dir));
strcpy(subdir->name, name);
subdir -> subdirs[0]=NULL;
subdir -> parentdir=currentdir;
subdir -> num_of_subdir=0;

currentdir->subdirs[currentdir->num_of_subdir]=subdir;
currentdir->num_of_subdir++;


}

void minirmdir(const char *dir_name){//shm내의 주소값 할당해제


int i;
for (i=0;i<currentdir->num_of_subdir;i++){
	if(!(currentdir->subdirs[i]==NULL))
	if(strcmp(currentdir->subdirs[i]->name,dir_name)==0){
		break;}}
		
if(currentdir->subdirs[i]==NULL){
printf("Directory not found\n");
return;
}

shfree(currentdir->subdirs[i]);

for(;currentdir->subdirs[i+1]!=NULL;i++){
currentdir->subdirs[i]=currentdir->subdirs[i+1];}

currentdir->subdirs[i]=NULL;

currentdir->num_of_subdir--;

}

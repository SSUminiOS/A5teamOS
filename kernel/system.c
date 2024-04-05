#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
void minisystem()
{
    printf("minisystem\n");
}



typedef struct dir{
char name[10];
struct dir* subdirs[5];
struct dir* parentdir;
int num_of_subdir;
}dir;

dir* currentdir;

void minimkroot(){
dir* root = shmalloc(sizeof(dir));
strcpy(root->name, "root");
root->subdirs[0]=NULL;
root->parentdir=root;
root->num_of_subdir=0;
currentdir=root;

printf("%ld", sizeof(dir));
}


void minicd(char *dir_name){
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

void minils(){
int i;
for (i=0;i<currentdir->num_of_subdir;i++){
	printf("%s ",currentdir->subdirs[i]->name);}
	
	printf("\n");
}

void minimkdir(const char *dir_name){
const char *name=dir_name;

dir* subdir = shmalloc(sizeof(dir));
strcpy(subdir->name, name);
subdir -> subdirs[0]=NULL;
subdir -> parentdir=currentdir;
subdir -> num_of_subdir=0;

currentdir->subdirs[currentdir->num_of_subdir]=subdir;
currentdir->num_of_subdir++;


}

void minirmdir(const char *dir_name){


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

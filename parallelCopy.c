#include	<fcntl.h>
#include	<unistd.h>
//#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>


#define SIZE 1024

typedef struct{
	unsigned long offset;
	unsigned long size;
}PART;

char file[50];
char fileout[50];

void *func1(void *arg);

int main(int argc, char *argv[]){
	char data[SIZE];
	struct stat f_stat;
	int fin1, fout1, x, chk, i = 0;
	PART part1, part2, part3;
	pthread_t t1, t2;
	
	if(argc < 3){
		puts("not enough arguments\n");
		puts("parallelCopy sourceFile targetFile");
		return -1;
	}
	strcpy(file, argv[1]);
	strcpy(fileout, argv[2]);
	stat(file, &f_stat);	//😀
	
	printf("size of file is %lu\n", f_stat.st_size);
	part1.offset = 0;
	
	// dividing the file in 3 parts
	part1.size = f_stat.st_size / 3;
	part2.offset = part1.size;
	part2.size = part1.size;
	part3.offset = part2.offset + part2.size;
	part3.size = f_stat.st_size - part3.offset;//😀
	
	/* creating part1 by main thread */
	fin1 = open(file, O_RDONLY);
	fout1 = open(fileout, O_WRONLY | O_CREAT, 0666);//🤔
	
	// creating 3 threads to copy 2 n 3rd part of file
	pthread_create(&t1, NULL, func1, &part2);
	pthread_create(&t2, NULL, func1, &part3);
	pthread_setname_np(t1, "THREAD1");//🦊
	pthread_setname_np(t2, "THREAD2");//🦊

	while(i < part1.size){
		x = read(fin1, data, SIZE);
		write(fout1, data, x);
		i += x;
	}
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("file is copied\n");
	close(fout1);
	close(fin1);
	return 0;
	
}

void *func1(void *arg)
{
	int fin, fout, x, i;
	PART *part;
	char data[SIZE];
	char tname[SIZE];
	
	part = (PART *)arg;
	fin = open(file, O_RDONLY);
	fout = open(fileout, O_WRONLY);//😀
	lseek(fin, part->offset, SEEK_SET);//😀
	lseek(fout, part->offset, SEEK_SET);//😀
	while(i < part->size){
		x = read(fin, data, SIZE);
		write(fout, data, x);
		i += x;
	}
	
	pthread_getname_np(pthread_self(), tname, SIZE);//🦊
	printf("%s is done\n", tname);
	close(fout);
	close(fin);
}

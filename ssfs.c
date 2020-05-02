#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#define MKDIR_STATUS 1
#define MKNOD_STATUS 2
#define RMDIR_STATUS 3
#define REMOVE_STATUS 4
#define READDR_STATUS 5
#define RENAME_STATUS 6
#define TRUNCATE_STATUS 7
#define WRITE_STATUS 8
#define READ_STATUS 9
#define OPEN_STATUS 10

static  const  char * dirpath = "/home/test/Documents";
char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
char encv1[10] = "encv1_";
char encv2[10] = "encv2_";
static int lastCommand = 0;

void encriptionLength(char* enc, int length) {
	if(strcmp(enc, ".") == 0 || strcmp(enc, "..") == 0)return;
	for(int i = length; i >= 0; i--){
		if(enc[i]=='/')break;
		if(enc[i]=='.'){
			length = i;
			break;
		}
	}
	int start = 0;
	for(int i = 0; i < length; i++){
		if(enc[i] == '/'){
			start = i+1;
			break;
		}
	}
    	for ( int i = start; i < length; i++) {
		if(enc[i]=='/')continue;
        	for (int j = 0; j < 87; j++) {
            		if(enc[i] == key[j]) {
                		enc[i] = key[(j+10) % 87];
                		break;
            		}
        	}
    	}
}

void decriptionLength(char * enc, int length){
	if(strcmp(enc, ".") == 0 || strcmp(enc, "..") == 0)return;
	if(strstr(enc, "/") == NULL)return;
	for(int i = length; i >= 0; i--){
		if(enc[i]=='/')break;
		if(enc[i]=='.'){
			length = i;
			break;
		}
	}
	int start = length;
	for(int i = 0; i < length; i++){
		if(enc[i] == '/'){
			start = i+1;
			break;
		}
	}
    	for ( int i = start; i < length; i++) {
		if(enc[i]=='/')continue;
        	for (int j = 0; j < 87; j++) {
            		if(enc[i] == key[j]) {
                		enc[i] = key[(j+77) % 87];
                		break;
            		}
        	}
    	}
}

void encriptionTwo(char * path){
	FILE * file = fopen(path, "rb");
	int count = 0;
	char topath[1000];
	sprintf(topath, "%s.%03d", path, count);
	void * buffer = malloc(1024);
	while(1){
		size_t readSize = fread(buffer, 1, 1024, file);
		if(readSize == 0)break;
		FILE * op = fopen(topath, "w");
		fwrite(buffer, 1, readSize, op);
		fclose(op);
		count++;
		sprintf(topath, "%s.%03d", path, count);
	}
	free(buffer);
	fclose(file);
	remove(path);
}

void encriptionTwoDir(char * dir){
	DIR *dp;
	struct dirent *de;
	dp = opendir(dir);
	if (dp == NULL)
		return;
	char dirPath[1000];
	char filePath[1000];
	while ((de = readdir(dp)) != NULL) {
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
			continue;
		}
		if(de->d_type == DT_DIR){
			sprintf(dirPath, "%s/%s", dir, de->d_name);
			encriptionTwoDir(dirPath);
		}
		else if(de->d_type == DT_REG){
			sprintf(filePath, "%s/%s", dir, de->d_name);
			encriptionTwo(filePath);
		}
	}
	closedir(dp);
}

void logInfo(char *input)
{
    FILE* fp;
    char data1[50];
    fp = fopen("/home/test/fs.log", "a");

    time_t raw;
    struct tm *timeinfo;
    char tanggal[40];
    time(&raw);
    timeinfo = localtime(&raw);

    strftime(tanggal, sizeof(tanggal), "%y%m%d-%H:%M:%S", timeinfo); 
    sprintf(data1,"INFO::%s::%s\n", tanggal, input);
    fputs(data1, fp);

    fclose(fp);
}

void logWarning(char *input)
{
    FILE* fp;
    char data1[50];
    fp = fopen("/home/test/fs.log", "a");

    time_t raw;
    struct tm *timeinfo;
    char tanggal[40];
    time(&raw);
    timeinfo = localtime(&raw);

    strftime(tanggal, sizeof(tanggal), "%y%m%d-%H:%M:%S", timeinfo); 
    sprintf(data1,"WARNING::%s::%s\n", tanggal, input);
    fputs(data1, fp);

    fclose(fp);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf){
	char * enc1Ptr = strstr(path, encv1);
	if(lastCommand == MKNOD_STATUS || lastCommand == MKDIR_STATUS){

	}
	else{
		if(enc1Ptr != NULL)
			decriptionLength(enc1Ptr, strlen(enc1Ptr));
	}
	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s", dirpath, path);
	res = lstat(fpath, stbuf);
	if (res == -1){
		return -errno;
	}
	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){

	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL)
		decriptionLength(enc1Ptr, strlen(enc1Ptr));
	char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	} 
	else sprintf(fpath, "%s%s", dirpath, path);

	int res = 0;
	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		if(enc1Ptr != NULL){
			char *strNama;
			strNama = de->d_name;
			encriptionLength(strNama, strlen(strNama));
		}
		res = (filler(buf, de->d_name, &st, 0));
		if(res!=0) break;
	}
	closedir(dp);
	lastCommand = READDR_STATUS;
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode){

	lastCommand = MKDIR_STATUS;
	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL){
		int length = strlen(enc1Ptr);
		for(int i = length; i >= 0; i--){
			if(enc1Ptr[i] == '/'){
				length = i;
				break;
			}
		}
		decriptionLength(enc1Ptr, length);
	}

	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);
	printf("%s\n", fpath);
	int res;

	res = mkdir(fpath, mode);
	char str[100];
	sprintf(str, "MKDIR::%s", path);
	logInfo(str);
	if (res == -1){
		return -errno;
	}
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev){

	lastCommand = MKNOD_STATUS;

	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL){
		int length = strlen(enc1Ptr);
		for(int i = length; i >= 0; i--){
			if(enc1Ptr[i] == '/'){
				length = i;
				break;
			}
		}
		decriptionLength(enc1Ptr, length);
	}

	char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	} else sprintf(fpath, "%s%s",dirpath,path);
	int res;

	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	}
	else if (S_ISFIFO(mode)){
		res = mkfifo(fpath, mode);
	}
	else{
		res = mknod(fpath, mode, rdev);
	}
	char str[100];
	sprintf(str, "CREAT::%s", path);
	logInfo(str);

	if (res == -1){
		return -errno;
	}

	return 0;
}

static int xmp_unlink(const char *path) {

	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL){
		decriptionLength(enc1Ptr, strlen(enc1Ptr));
	}

	char fpath[1000];
	if(strcmp(path,"/") == 0){
		path=dirpath;
		sprintf(fpath,"%s",path);
	} 
	else sprintf(fpath, "%s%s",dirpath,path);
	int res;

	char str[100];
	sprintf(str, "REMOVE::%s", path);
	logWarning(str);

	res = unlink(fpath);
	if (res == -1){
		return -errno;
	}
	lastCommand = REMOVE_STATUS;
	return 0;
}

static int xmp_rmdir(const char *path) {
	lastCommand = RMDIR_STATUS;

	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL){
		decriptionLength(enc1Ptr, strlen(enc1Ptr));
	}

	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);
	int res;

	res = rmdir(fpath);
	char str[100];
	sprintf(str, "RMDIR::%s", path);
	logWarning(str);
	if (res == -1){
		return -errno;
	}
	return 0;
}

static int xmp_rename(const char * from, const char * to) {
	lastCommand = RENAME_STATUS;

	char from2[1000];
	char to2[1000];
	sprintf(from2, "%s%s",dirpath, from);
	sprintf(to2, "%s%s",dirpath, to);
	int res;

	int dirIndex = 0;
	int length = strlen(to2);
	for(int i = length; i >= 0; i--){
		if(to2[i] == '/'){
			dirIndex = i;
			break;
		}
	}

	char dir[1000];
	strncpy(dir, to2, dirIndex);
	pid_t id = fork();
	if(id){
		wait(NULL);
	}
	else{
		char * arg[]={"mkdir", "-p", dir, NULL};
		execv("/bin/mkdir", arg);
	}

	res = rename(from2, to2);
	char str[100];
	sprintf(str, "RENAME::%s::%s", from, to);
	logInfo(str);
	if (res == -1){
		return -errno;
	}
	return 0;
}

static int xmp_truncate(const char *path, off_t size) {
	lastCommand = TRUNCATE_STATUS;

	char * enc1Ptr = strstr(path, encv1);
	if(enc1Ptr != NULL){
		decriptionLength(enc1Ptr, strlen(enc1Ptr));
	}

	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,path);
	int res;

	res = truncate(fpath, size);
	if (res == -1){
		return -errno;
	}
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi){

	char * enc1Ptr = strstr(path, encv1);
	if(lastCommand == MKNOD_STATUS){
		if(enc1Ptr != NULL){
			int length = strlen(enc1Ptr);
			for(int i = length; i >= 0; i--){
				if(enc1Ptr[i] == '/'){
					length = i;
					break;
				}
			}
			decriptionLength(enc1Ptr, length);
		}
	}
	else{
		if(enc1Ptr != NULL){
			decriptionLength(enc1Ptr, strlen(enc1Ptr));
		}
	}

	char fpath[1000];

	sprintf(fpath, "%s%s",dirpath,path);

	int res;

	res = open(fpath, fi->flags);
	if (res == -1){
		return -errno;
	}
	close(res);
	return 0;
}

static int xmp_read(const char * path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char * enc1Ptr = strstr(path, encv1);
	if(lastCommand == MKNOD_STATUS){
		if(enc1Ptr != NULL){
			int length = strlen(enc1Ptr);
			for(int i = length; i >= 0; i--){
				if(enc1Ptr[i] == '/'){
					length = i;
					break;
				}
			}
			decriptionLength(enc1Ptr, length);
		}
	}
	else{
		if(enc1Ptr != NULL){
			decriptionLength(enc1Ptr, strlen(enc1Ptr));
		}
	}
	lastCommand = READ_STATUS;

	char fpath[1000];
	int fd;
	int res = 0;

	(void) fi;
	sprintf(fpath, "%s%s",dirpath,path);

	fd = open(fpath, O_RDONLY);
	if (fd == -1){
		return -errno;
	}

	res = pread(fd, buf, size, offset);
	if (res == -1){
		res = -errno;
	}
	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

	char * enc1Ptr = strstr(path, encv1);
	if(lastCommand == MKNOD_STATUS){
		if(enc1Ptr != NULL){
			int length = strlen(enc1Ptr);
			for(int i = length; i >= 0; i--){
				if(enc1Ptr[i] == '/'){
					length = i;
					break;
				}
			}
			decriptionLength(enc1Ptr, length);
		}
	}
	else{
		if(enc1Ptr != NULL){
			decriptionLength(enc1Ptr, strlen(enc1Ptr));
		}
	}

	lastCommand = WRITE_STATUS;

	char fpath[1000];
	sprintf(fpath, "%s%s", dirpath, path);
	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);

	if (fd == -1){
		return -errno;
	}

	char str[100];
	sprintf(str, "WRITE::%s", path);
	logInfo(str);

	res = pwrite(fd, buf, size, offset);
	if (res == -1){
		res = -errno;
	}
	close(fd);
	return res;
}


static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.open = xmp_open,
	.read = xmp_read,
	.write = xmp_write,

};

int  main(int  argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

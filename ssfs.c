#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

static const char *dirpath = "/home/test/Documents";

int i,ii;
char *stringnama;
char ext_cae[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

void encae(char *input){
	if(!strcmp(input,".") || !strcmp(input,"..")) return;

	for(i=0;i<strlen(input);i++)
	{
		for(ii=0;ii<strlen(ext_cae);ii++){
			if(input[i]=='.')
			{
				return;
			}
			else if(input[i]==ext_cae[ii]){
				input[i] = ext_cae[(ii+17)%strlen(ext_cae)];
				break;
			}
		}
	}
}

void decae(char *input){
	if(!strcmp(input,".") || !strcmp(input,"..")) return;

	for(int i=0;i<strlen(input);i++)
	{
		for(int ii=0;ii<strlen(ext_cae);ii++){
                	if(input[i]=='.')
                        {
                        	return;
                        }
			else if(input[i]==ext_cae[ii]){
				input[i] = ext_cae[(ii+strlen(ext_cae)-17)%strlen(ext_cae)];
				break;
			}
		}
	}
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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char filepath[121];
	sprintf(filepath, "%s", path);
	encae(filepath);
  	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,filepath);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char filepath[121];
	sprintf(filepath, "%s", path);

	encae(filepath);
  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,filepath);
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

                stringnama=de->d_name;
		decae(stringnama);

		res = (filler(buf, stringnama, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char filepath[121];
	sprintf(filepath, "%s", path);

        encae(filepath);

  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,filepath);

	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char filepath[121];
	char input[121];
	sprintf(filepath, "%s", path);

	decae(filepath);
	int fd;
	int res;

  	char fpath[1000];
  	sprintf(fpath,"%s%s",dirpath,filepath);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	sprintf(input,"WRITE::%s",path);
	logInfo(input);

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	char filepath[121];
	char input[121];
	sprintf(filepath, "%s", path);

        encae(filepath);
  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,filepath);
	res = mkdir(fpath, mode);
	sprintf(input,"MKDIR::%s",path);
	logInfo(input);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
	char filepath[121];
	char input[121];
	sprintf(filepath, "%s", path);

        encae(filepath);
  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,filepath);
	res = rmdir(fpath);
	sprintf(input,"RMDIR::%s",path);
	logWarning(input);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
	char input[121];
	res = rename(from, to);
	sprintf(input,"RENAME::%s::%s",from,to);
	logInfo(input);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
	char filepath[121];
	char input[121];
	sprintf(filepath, "%s", path);
        encae(filepath);

  	char fpath[1000];
	sprintf(fpath, "%s%s",dirpath,filepath);
	res = unlink(fpath);
	sprintf(input,"UNLINK::%s",path);
	logWarning(input);
	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
  	.write 		= xmp_write,
	.mkdir		= xmp_mkdir,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.unlink		= xmp_unlink,

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

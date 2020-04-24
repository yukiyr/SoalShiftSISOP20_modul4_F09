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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char filepath[121];
	sprintf(filepath, "%s", path);
	char * pin = strstr(path,"encv1_");
	if(pin!=NULL){
		decae(pin);
	}
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
        char * pin = strstr(path,"encv1_");
	if(pin!=NULL){
		decae(pin);
	}
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
		if(pin!=NULL){
			encae(stringnama);
		}
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
        //int result;
        //char compare[20];
        //strcpy(compare, "encv1_");
        //char* compare1 = getFilename(filepath);
        //result = strcmp(compare,compare1);
        //if(result == 0)
        //{
        char * pin = strstr(path,"encv1_");
	if(pin!=NULL){
        	decae(pin);
	}
        //}

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
	sprintf(filepath, "%s", path);
        char * pin = strstr(path,"encv1_");
	if(pin!=NULL){
		decae(pin);
	}
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

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
  	.write 		= xmp_write,

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

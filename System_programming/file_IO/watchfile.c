#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


void readfile(char *path)
{
	struct dirent * dp; 
	char buf[1024];
	DIR * dir = opendir(path);
	if(dir == NULL)
		perror("opendir error!");

	while((dp = readdir(dir)) != NULL)
	{

		if(dp->d_name[0] == '.')  // 防止无限递归
			continue;
		// if(S_ISDIR(st.st_mode))
		// {
		// 	sprintf(buf, "%s/%s", path, dp->d_name);
		// 	readfile(buf);
		// }else if(S_ISREG(st.st_mode)){
		// 	printf("%10s\t\t%ld\n", path, st.st_size);
		// 	printf("%s \t", dp->d_name);
		// }
		// printf("%10s\t\t%ld\n", path, st.st_size);
		sprintf(buf, "%s/%s", path, dp->d_name);
		struct stat st;
		int res = stat(buf, &st);
		if(res == -1)
			perror("stat error!");
		if(S_ISDIR(st.st_mode))
		{
			readfile(buf);
		}else if(S_ISREG(st.st_mode)){
			printf("%s \n", buf);
		}
		
	}
	closedir(dir);
}

int main(int argc, char * argv[])
{
	struct stat st;
	int res;

	res = stat(argv[1], &st);
	if(res == -1)
		perror("stat error!");

	if(S_ISDIR(st.st_mode))
	{
		readfile(argv[1]);
	}else if(S_ISREG(st.st_mode)){
		printf("%10s\t\t%ld\n", argv[1], st.st_size);
	}

	return 0;
}

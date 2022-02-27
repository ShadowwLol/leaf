#include "../include/leaf.h"
#include <dirent.h>

FN list_files(const char * path){
	int32_t n = 0;
	DIR * d = opendir(path);
	struct dirent * dir;

	while((dir = readdir(d))){
		if (dir->d_name[0] != '.' && MEL_strlen(dir->d_name)){
			++n;
			printf("%d. [%s]\n", n, dir->d_name);
		}
	}

	rewinddir(d);

	if (n < 1){
		MEL_log(LOG_ERROR, "No accounts in filesystem");
		return EX_F;
	}

	return EX_S;
}

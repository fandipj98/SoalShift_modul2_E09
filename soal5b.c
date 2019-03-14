#include <sys/types.h>
#include <unistd.h>

int main(){

	char *argv[4] = {"pkill", "-INT", "soal5", NULL};
    execv("/usr/bin/pkill", argv);
 
	return 0;
}
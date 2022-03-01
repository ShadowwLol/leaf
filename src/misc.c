#include "../include/leaf.h"
#include "../include/hash.h"
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

#if __WIN32
#define mkdir(A, B) mkdir(A)
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

bool path_exists(const char * path){
	struct stat st = {0};
	if (stat(path, &st) == -1 ){
		return false;
	}
	return true;
}

/*
	.leafcfg/
		details.lf
		accounts/
			xxx.lf
			yyy.lf
			zzz.lf
*/

FN startup(unsigned char * key, unsigned char * iv, char * cfg_path, char * acc_path){
	MEL_log(LOG_INFORMATION, "Creating configuration..");

	mkpath(acc_path, 0755);

	char pass1[1024] = "\0";
	char pass2[1024] = "\0";
	while (1){

		MEL_set_color(TERM_YELLOW);
		printf("Enter Master Password: ");
		MEL_clear_color();

		MEL_echo(false);
		if (EX_F == get_input(pass1, 1024, stdin)){
			return EX_F;
		}
		MEL_echo(true);
		putchar('\n');

		if (MEL_strlen(pass1) < 5){
			continue;
		}

		MEL_set_color(TERM_YELLOW);
		printf("Re-enter Master Password: ");
		MEL_clear_color();

		MEL_echo(false);
		if (EX_F == get_input(pass2, 1024, stdin)){
			return EX_F;
		}
		MEL_echo(true);
		putchar('\n');

		if (0 == strncmp(pass1, pass2, 1024)){
			break;
		}
	}

	/* Hashing Master-Password */
	char hash[SHA512_OUT_LENGTH] = "\0";
	sha512_string(pass1, hash);

	sha256_string(pass1, (char *)key);
	sha256_string((char *)key, (char *)iv);

	/* Writing to configuration file */
	FILE * fp = fopen(cfg_path, "w");
	fprintf(fp, "[master]\npassword = \"%s\"", hash);
	fclose(fp);

	return EX_S;
}

FN load(unsigned char * key, unsigned char * iv, char * cfg_path){
	MEL_log(LOG_INFORMATION, "Loading configuration..");
	/* Reading from configuration file */
	ini_t *config = ini_load(cfg_path);

	const char * password = ini_get(config, "master", "password");
	if (!password){
		MEL_log(LOG_ERROR, "Failed getting master password from configuration file");
		ini_free(config);
		return EX_F;
	}

	uint8_t attempts = 4;
	char pass[1024] = "\0";
	while (--attempts){
		MEL_set_color(TERM_YELLOW);
		printf("Enter Master Password: ");
		MEL_clear_color();

		MEL_echo(false);
		if (EX_F == get_input(pass, 1024, stdin)){
			return EX_F;
		}
		MEL_echo(true);
		putchar('\n');

		/* Hashing Master-Password */
		char hash[SHA512_OUT_LENGTH] = "\0";
		sha512_string(pass, hash);

		if (0 == strncmp(password, hash, 1024)){
			sha256_string(pass, (char *)key);
			sha256_string((char *)key, (char *)iv);
			return EX_S;
		}
	}

	return EX_F;
}

const char * get_home_dir(){
#if __WIN32
	return getenv("USERPROFILE");
#else
	if (!getenv("HOME")){
	    return getpwuid(getuid())->pw_dir;
	}
	return getenv("HOME");
#endif
}

#if __WIN32

#include <windef.h>
#include <winbase.h>
#include <wincon.h>

void MEL_echo(bool display) {
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    if(hStdIn == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "GetStdHandle failed (error %lu)\n", GetLastError());
        return;
    }

    /* Get console mode */
    DWORD mode;
    if(!GetConsoleMode(hStdIn, &mode)) {
        fprintf(stderr, "GetConsoleMode failed (error %lu)\n", GetLastError());
        return;
    }

    if(display) {
        /* Add echo input to the mode */
        if(!SetConsoleMode(hStdIn, mode | ENABLE_ECHO_INPUT)) {
            fprintf(stderr, "SetConsoleMode failed (error %lu)\n", GetLastError());
            return;
        }
    }
    else {
        /* Remove echo input to the mode */
        if(!SetConsoleMode(hStdIn, mode & ~((DWORD) ENABLE_ECHO_INPUT))) {
            fprintf(stderr, "SetConsoleMode failed (error %lu)\n", GetLastError());
            return;
        }
    }
}

#else

#include <termios.h>

void MEL_echo(bool display) {
    struct termios t;

    /* Get console mode */
    errno = 0;
    if (tcgetattr(STDIN_FILENO, &t)) {
        fprintf(stderr, "tcgetattr failed (%s)\n", strerror(errno));
        return;
    }

    /* Set console mode to echo or no echo */
    if (display) {
        t.c_lflag |= ECHO;
    } else {
        t.c_lflag &= ~((tcflag_t) ECHO);
    }
    errno = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &t)) {
        fprintf(stderr, "tcsetattr failed (%s)\n", strerror(errno));
        return;
    }
}

#endif

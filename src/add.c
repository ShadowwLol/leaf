#include "../include/leaf.h"

FN add(const char * acc_path, const char * account){
	if (MEL_strlen(account) < 5){
		return EX_F;
	}
	String file_path = init_str();
	set_str(&file_path, acc_path);
	append_str(&file_path, account);
	append_str(&file_path, ".lf");

	if (path_exists(file_path.buffer)){
		MEL_log(LOG_ERROR, "Account already exists {%s} => {%s}", account, file_path.buffer);
		remove_str(&file_path);
		return EX_F;
	}

	char username[1024] = "\0";
	char pass1[1024] = "\0";
	char pass2[1024] = "\0";
	while (1){
		MEL_set_color(TERM_YELLOW);
		printf("Enter username: ");
		MEL_clear_color();

		if (EX_F == get_input(username, 1024, stdin)){
			remove_str(&file_path);
			return EX_F;
		}

		if (MEL_strlen(username) < 5){
			continue;
		}

		MEL_set_color(TERM_YELLOW);
		printf("Enter password: ");
		MEL_clear_color();

		MEL_echo(false);
		if (EX_F == get_input(pass1, 1024, stdin)){
			MEL_echo(true);
			remove_str(&file_path);
			return EX_F;
		}
		MEL_echo(true);
		putchar('\n');

		if (MEL_strlen(pass1) < 5){
			continue;
		}

		MEL_set_color(TERM_YELLOW);
		printf("Re-enter password: ");
		MEL_clear_color();

		MEL_echo(false);
		if (EX_F == get_input(pass2, 1024, stdin)){
			MEL_echo(true);
			remove_str(&file_path);
			return EX_F;
		}
		MEL_echo(true);
		putchar('\n');

		if (0 == strncmp(pass1, pass2, 1024)){
			break;
		}

	}

	FILE * fp = fopen(file_path.buffer, "w");
	fprintf(fp, "[account]\nusername = \"%s\"\npassword = \"%s\"", username, pass1);
	fclose(fp);

	MEL_log(LOG_SUCCESS, "Successfully created account {%s} => {%s}", account, file_path.buffer);

	remove_str(&file_path);

	return EX_S;
}

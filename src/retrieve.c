#include "../include/leaf.h"

FN retrieve(const char * acc_path, const char * account){
	if (MEL_strlen(account) < 5){
		return EX_F;
	}
	String path = init_str();
	set_str(&path, acc_path);
	append_str(&path, account);
	append_str(&path, ".lf");

	if (!path_exists(path.buffer)){
		MEL_log(LOG_ERROR, "Failed retrieving account {%s} as {%s} does not exist", account, path.buffer);
		remove_str(&path);
		return EX_F;
	}

	ini_t *config = ini_load(path.buffer);

	const char * username = ini_get(config, "account", "username");
	if (!username){
		MEL_log(LOG_ERROR, "Failed retrieving username from {%s} account", account);
		ini_free(config);
		remove_str(&path);
		return EX_F;
	}

	const char * password = ini_get(config, "account", "password");
	if (!password){
		MEL_log(LOG_ERROR, "Failed retrieving password from {%s} account", account);
		ini_free(config);
		remove_str(&path);
		return EX_F;
	}

	ini_free(config);

	MEL_set_color(TERM_YELLOW);
	printf("Username: ");
	MEL_clear_color();
	printf("%s\n", username);

	MEL_set_color(TERM_YELLOW);
	printf("Password: ");
	MEL_clear_color();
	printf("%s\n", password);

	MEL_log(LOG_SUCCESS, "Successfully retrieved account {%s} => {%s}", account, path.buffer);

	remove_str(&path);
	return EX_S;
}

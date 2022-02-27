#include "../include/leaf.h"

FN delete(const char * acc_path, const char * account){
	if (MEL_strlen(account) < 5){
		return EX_F;
	}
	String path = init_str();
	set_str(&path, acc_path);
	append_str(&path, account);
	append_str(&path, ".lf");

	if (!path_exists(path.buffer)){
		MEL_log(LOG_ERROR, "Failed deleting account {%s} as {%s} does not exist", account, path.buffer);
		remove_str(&path);
		return EX_F;
	}

	remove(path.buffer);

	MEL_log(LOG_SUCCESS, "Successfully deleted account {%s} => {%s}", account, path.buffer);

	remove_str(&path);
	return EX_S;
}

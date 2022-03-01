#include "../include/leaf.h"
#include "../include/hash.h"

int32_t main(int32_t argc, char * argv[]){
	MEL_set_color(TERM_YELLOW);
	fprintf(stdout, "%s\n", EXEC);
	MEL_clear_color();

	unsigned char key[SHA256_OUT_LENGTH] = "\0";
	unsigned char iv[SHA256_OUT_LENGTH]   = "\0";

	String acc_path = init_str();
	set_str(&acc_path, get_home_dir());

	String cfg_path = init_str();
	set_str(&cfg_path, acc_path.buffer);
	append_str(&cfg_path, "/.config/leafcfg/details.lf");

	append_str(&acc_path, "/.config/leafcfg/accounts/");

	if (path_exists(acc_path.buffer)){
		if (EX_F == load(key, iv, cfg_path.buffer)){
			remove_str(&cfg_path);
			remove_str(&acc_path);
			return EX_F;
		}
	}else{
		if (EX_F == startup(key, iv, cfg_path.buffer, acc_path.buffer)){
			remove_str(&cfg_path);
			remove_str(&acc_path);
			return EX_F;
		}
	}

	key[KEY_LENGTH] = '\0';
	iv[IV_LENGTH] = '\0';
	printf("key: [%s]\niv: [%s]\n",key, iv);

	remove_str(&cfg_path);

	static struct option long_options[] = {
		/* These options set a flag. */
		{"help",        no_argument,            0, 'h'},
		{"list",        no_argument,            0, 'l'},
		{"add",         required_argument,      0, 'a'},
		{"retrieve",    required_argument,      0, 'r'},
		{"delete",      required_argument,      0, 'd'},
		{0, 0, 0, 0}
	};

	int32_t option_index = 0;
	int32_t c = getopt_long(argc, argv, "hla:r:d:", long_options, &option_index);
	if (c == -1){
		return EX_F;
	}
	switch (c){
		default:
		case 'h':
			MEL_log(LOG_INFORMATION, "help menu:\n"\
									 "\t--help, -h\n"\
									 "\t\tDisplay this help menu\n"\
									 "\t--list, -l\n"\
									 "\t\tList all accounts\n"\
									 "\t--add, -a <account>\n"\
									 "\t\tAdd an account\n"\
									 "\t--retrieve, -r <account>\n"\
									 "\t\tRetrieve an account\n"\
									 "\t--delete, -d <account>\n"\
									 "\t\tDelete an account\n");
			break;

		case 'l':
			MEL_log(LOG_INFORMATION, "All Accounts:\n");
			if (EX_F == list_files(acc_path.buffer)){
				remove_str(&acc_path);
				return EX_F;
			}
			break;

		case 'a':
			MEL_log(LOG_INFORMATION, "Add an account: {%s}", optarg);
			if (EX_F == add(acc_path.buffer, optarg)){
				remove_str(&acc_path);
				return EX_F;
			}
			break;

		case 'r':
			MEL_log(LOG_INFORMATION, "Retrieve an account: {%s}", optarg);
			if (EX_F == retrieve(acc_path.buffer, optarg)){
				remove_str(&acc_path);
				return EX_F;
			}
			break;

		case 'd':
			MEL_log(LOG_INFORMATION, "Delete an account: {%s}", optarg);
			if (EX_F == delete(acc_path.buffer, optarg)){
				remove_str(&acc_path);
				return EX_F;
			}
			break;
	}

	remove_str(&acc_path);
	return EX_S;
}

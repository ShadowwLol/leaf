#ifndef _LEAF_H
#define _LEAF_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include "MEL_IO.h"
#include "MEL_logs.h"

#include "ext/ini.h"

#define FN uint8_t
#define EX_S (0)
#define EX_F (255)
#define EXEC "./leaf"

#define KEY_LENGTH (32)
#define IV_LENGTH  (16)

const char * get_home_dir();
void MEL_echo(bool display);
bool path_exists(const char * path);
FN startup(unsigned char * key, unsigned char * iv, char * cfg_path, char * acc_path);
FN load(unsigned char * key, unsigned char * iv, char * cfg_path);

FN list_files(const char * path);
FN add(const char * acc_path, const char * account);
FN retrieve(const char * acc_path, const char * account);
FN delete(const char * acc_path, const char * account);

#endif /* _LEAF_H */

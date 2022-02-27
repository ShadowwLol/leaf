#ifndef _PSW_H
#define _PSW_H

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

const char * get_home_dir();
void MEL_echo(bool display);
bool path_exists(const char * path);
FN startup(char * cfg_path, char * acc_path);
FN load(char * cfg_path);

FN list_files(const char * path);
FN add(const char * acc_path, const char * account);
FN retrieve(const char * acc_path, const char * account);
FN delete(const char * acc_path, const char * account);

#endif /* _PSW_H */

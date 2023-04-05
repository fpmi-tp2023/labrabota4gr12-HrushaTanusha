#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName);

#endif /* FUNCTIONS_H_ */
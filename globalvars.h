#ifndef __GLOBALVARS_H
#define __GLOBALVARS_H

#define GLOBAL_VARS				\
VAR(int, GRID_W, 16)			\
VAR(int, GRID_H, 16)			\
VAR(int, MINE_NUM, 40)			\
VAR(int, WRAP_AROUND, 1)

#define VAR(type, name, default) extern type name;

GLOBAL_VARS

const char *const FILENAME = "options.txt";
const char *const FIRST_LINE = "Minesweeper: Options file";

void readOptions();
void saveOptions();

#endif // __GLOBALVARS_H

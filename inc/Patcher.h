/// @file Patcher.h

#ifndef PATCHER_H
#define PATCHER_H

#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include <stdio.h>

typedef struct
{
    char original    = 0;
    int  place       = 0;
    char replacement = 0;
} PATCH_DATA;

const int  PATCH_STOP_SYMB = -1;
const bool PATCH_FILE_ERR  = 0;
const bool PATCH_FILE_SUCC = 1;

const char STOP_SYMB = ';';

int             PatchingComFile         (const char * com_file_name, PATCH_DATA * patch_dt);
char *          MakeComFileBuf          (const char * com_file_name, FILE * com_file, size_t * com_file_size);
PATCH_DATA *    GetPatchFileData        (const char * patch_file_name);
size_t          CountPatchSymbols       (char * buff_pointer);
PATCH_DATA *    FillPatchStruct         (char * buff_pointer);
int             CheckFileBeforPatching  (char * com_file_buf, PATCH_DATA * patch_file_buf);

#endif



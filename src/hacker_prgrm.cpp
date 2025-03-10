#include "../inc/Patcher.h"
#include "../inc/Utils.h"
//debum flat
//-------------------------------------------------------------------------------------------------
//                                    CONSTANTS & TYPEDEF

const char * PATCH_FILE_NAME = "./inc/Patch.txt";
const char * COM_FILE_NAME   = "../dan.com";

//-------------------------------------------------------------------------------------------------

int main ()
{
    PATCH_DATA * patch_dt = GetPatchFileData (PATCH_FILE_NAME);

    PatchingComFile (COM_FILE_NAME, patch_dt);
}

int PatchingComFile (const char * com_file_name, PATCH_DATA * patch_dt)
{
    assert (com_file_name && patch_dt);

    FILE * com_file = fopen (com_file_name, "rb");
    size_t com_file_size = 0;

    char * com_file_buf = MakeComFileBuf (com_file_name, com_file, &com_file_size);

    if (!CheckFileBeforPatching (com_file_buf, patch_dt))
    {
        assert (!fclose (com_file));

        free (patch_dt);
        free (com_file_buf);

        return 0;
    }

    assert (!fclose (com_file));
    FILE * write_to_com_file = fopen (com_file_name, "w");

    size_t ptr = 0;
    while (patch_dt[ptr].place != PATCH_STOP_SYMB)
    {
        com_file_buf[patch_dt[ptr].place] = patch_dt[ptr].replacement;

        ptr++;
    }

    fwrite (com_file_buf, sizeof (char), com_file_size, write_to_com_file);

    assert (!fclose (com_file));

    free (patch_dt);

    return 0;
}

char * MakeComFileBuf (const char * com_file_name, FILE * com_file, size_t * com_file_size)
{
    assert (com_file_name);

    struct stat com_stats;

    assert (!stat (com_file_name, &com_stats));
    *com_file_size = com_stats.st_size;

    COLOR_PRINT (GREEN, "%d - size com\n", *com_file_size);

    char * com_file_buf = (char *) calloc (sizeof (char), *com_file_size);
    assert (com_file_buf);

    COLOR_PRINT (RED, "befor setvbuf\n");

    assert (fread (com_file_buf, sizeof (char), *com_file_size, com_file));

    return com_file_buf;
}

PATCH_DATA * GetPatchFileData (const char * patch_file_name)
{
    assert (patch_file_name);

    struct stat patch_stat;

    FILE * patch_file = fopen (patch_file_name, "rb");
    assert (patch_file);

    COLOR_PRINT (YELLOW, "after stst patch\n");

    assert (!stat (patch_file_name, &patch_stat));
    size_t patch_file_size = patch_stat.st_size;

    COLOR_PRINT (GREEN, "%d - size patch\n", patch_file_size);

    COLOR_PRINT (YELLOW, "after stst patch\n");
    
    char * patch_data_buf = (char *) calloc (patch_file_size, sizeof (char));
    assert (patch_data_buf);

    assert (fread (patch_data_buf, sizeof (char), patch_file_size, patch_file));

    assert (!fclose (patch_file));

    PATCH_DATA * patch_dt = FillPatchStruct (patch_data_buf);

    free (patch_data_buf);

    return patch_dt;           
}

size_t CountPatchSymbols (char * buff_pointer)
{
    assert (buff_pointer);

    char * ptr  = buff_pointer;
    int counter = 0;

    while (*ptr != STOP_SYMB)                                         // While it is not stop symbol
    {
        if (*ptr == 'x')
            counter++;

        ptr++;
    }

    counter /= 3;
    counter++;                                                        // Last stop-struct

    return counter;
}

PATCH_DATA * FillPatchStruct (char * buff_pointer)
{
    assert (buff_pointer);

    char * cpy_buff_pointer = buff_pointer;

    size_t n_patch_structs = CountPatchSymbols (cpy_buff_pointer);

    PATCH_DATA * patch_dt = (PATCH_DATA *) calloc (n_patch_structs, sizeof (PATCH_DATA));
    assert (patch_dt);

    while (*cpy_buff_pointer != '\n')
        cpy_buff_pointer++;
    
    for (size_t i = 0; i < n_patch_structs - 1; i++)
    {
        patch_dt[i].original    = (char) strtol (cpy_buff_pointer, &cpy_buff_pointer, 16);
        patch_dt[i].place       = strtol (cpy_buff_pointer, &cpy_buff_pointer, 16);
        patch_dt[i].replacement = (char) strtol (cpy_buff_pointer, &cpy_buff_pointer, 16);

        printf ("orig %c\nplace %x\nreplace %c\n\n", patch_dt[i].original, patch_dt[i].place, patch_dt[i].replacement);
    }

    printf ("n_patch_structks = %d\n", n_patch_structs);

    patch_dt[n_patch_structs - 1].original    = PATCH_STOP_SYMB;
    patch_dt[n_patch_structs - 1].place       = PATCH_STOP_SYMB;
    patch_dt[n_patch_structs - 1].replacement = PATCH_STOP_SYMB;

    return patch_dt;
}

int CheckFileBeforPatching (char * com_file_buf, PATCH_DATA * patch_file_buf)
{
    assert (com_file_buf && patch_file_buf);

    COLOR_PRINT (YELLOW, "Start checking COM file befor patching...\n");

    size_t ptr = 0;

    while (patch_file_buf[ptr].place != PATCH_STOP_SYMB)
    {
        if (com_file_buf[patch_file_buf[ptr].place] == patch_file_buf[ptr].replacement)
        {
            COLOR_PRINT (CYAN, "Warning! Place <%d> alraedy patched or matches\n", patch_file_buf[ptr].place);
        }
        else if (com_file_buf[patch_file_buf[ptr].place] != patch_file_buf[ptr].original)
        {
            COLOR_PRINT (RED, "ERROR! COM file does not match with expected file.\n"
                              "Coords: %d\tNum in file: %c\t\tExpended: %c\n"
                              "\nBreaking...\n", patch_file_buf[ptr].place, com_file_buf[patch_file_buf[ptr].place], patch_file_buf[ptr].original);

            return PATCH_FILE_ERR;
        }

        COLOR_PRINT (GREEN, "OK\n");

        ptr++;
    }

    COLOR_PRINT (GREEN, "COM file checked successfuly. Start patching...\n");

    return PATCH_FILE_SUCC;
}
//Mmap
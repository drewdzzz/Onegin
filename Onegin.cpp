/// @file


// Log of changes:
//-----------------------------------------------------------------------------
//Version 1.1
//
//Comporator doesn't change strings
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/// @brief Function calculates size of file
/// @param *stream Stream with file
/// @return Size of file
long  size_of_file ( FILE* stream);

/// @brief Function counts strings in array with text
/// @param *poem_arr Array containing text
/// @return Number of strings
long  stringcount ( char* poem_arr );

/// @brief Function chages \\n symbols to \\0
/// @param *poem_arr Array with text
/// @param number_of_strings
void  stringmaker ( char* poem_arr, long number_of_strings );

/// @brief Function assigns pointers to every string in array with text
/// @param *poem_arr Array containing text
/// @param **pointers Array with pointers
/// @param number_of_strings Number of strings
void  makeptr ( char* poem_arr, char** pointers, long number_of_strings);

/// @brief Compare 2 strings
/// @return Returns difference between first mismatching symbols in strings or returns 0 if first string if equal second one
int   strcmp ( const char* string1, const char* string2 );

/// @brief Swap 2 strings in an array
/// @param mySwappingArray Array with strings
/// @param a Index of first string
/// @param b Index of second string
void  mySwap ( char** mySwappingArray, long a, long b);

/// @brief An usual quicksort, but for strings:)
/// @param **arr Array for sorting
/// @param left Lower edge for sort
/// @param right Upper edge for sort
void  quicksort ( char** arr, long left, long right);

/// @brief Opens file (You must write the name of file)
/// @return Stream with file
FILE* open_file ();

/// @brief Writes strings in file
/// @param **stringpointer Array containing pointers to strings
/// @param **number_of_strings Number of strings;
void write_in_file ( char** stringpointer, long number_of_strings);


/// @param  [in] ch A symbol
/// @return ch if it is a lowercase letter or lowercase analogue of ch
char lowercase_letter (char ch);

int main()
{
    FILE* stream = open_file ();
    if (!stream) return 1;
    long file_size = size_of_file ( stream );
    char* poem_arr = (char*) calloc ( file_size+1, sizeof(char*) );
    if ( !poem_arr )
    {
        printf ("Memory can't be allocated\n");
        return 1;
    }
    long number_of_symbols = fread ( poem_arr, sizeof(char), file_size, stream );
    poem_arr[number_of_symbols] = '\0';
    fclose (stream);
    long number_of_strings = stringcount ( poem_arr );
    stringmaker ( poem_arr, number_of_strings );
    char** stringpointer = (char**)calloc ( number_of_strings, sizeof (char*) );
    makeptr (poem_arr, stringpointer, number_of_strings);
    quicksort (stringpointer, 0, number_of_strings-1);
    write_in_file ( stringpointer, number_of_strings);
    free(stringpointer);
    free(poem_arr);
    return 0;
}

FILE* open_file()
{
    FILE *stream;
    char INPUT_FILE_NAME[75] = "";
    scanf("%s", INPUT_FILE_NAME);
    if ( !( stream = fopen ( INPUT_FILE_NAME, "r") ) )
    {
        fprintf (stderr, "Input file is not open\n");
        return 0;
    }
    return stream;
}

void write_in_file( char** stringpointer, long number_of_strings)
{
    assert (stringpointer);
    assert ( isfinite (number_of_strings) );
    char OUTPUT_FILE_NAME[75] = "";
    scanf ("%s", OUTPUT_FILE_NAME);
    FILE* stream;
    if ( !( stream = fopen ( OUTPUT_FILE_NAME, "w") ) )
    {
        fprintf (stderr, "Input file is not open\n");
        return;
    }
    for ( int i = 0; i < number_of_strings; i++) fprintf(stream, "%s\n", *stringpointer++ );
    fclose (stream);
}

long size_of_file (FILE* stream)
{
    assert ( stream );

    fseek ( stream, 0, SEEK_END);
    long filesize = ftell (stream) * sizeof(char*);
    fseek ( stream, 0, SEEK_SET);
    return filesize;
}

long stringcount ( char* poem_arr )
{
    assert ( *poem_arr );
    assert (  poem_arr );

    long scount = 0;
    while (*poem_arr)
    {
       if ( *poem_arr == '\n' )
        {
            *poem_arr = '\0';
            scount++;
        }
        poem_arr++;
    }
    return ++scount;
}

void stringmaker ( char* poem_arr, long number_of_strings )
{
    assert ( poem_arr );
    assert ( *poem_arr);
    assert ( isfinite ( number_of_strings ) );

    for ( long i = 1; i < number_of_strings; i++)
    {
        if ( poem_arr[i] == '\n' ) poem_arr[i] = '\0';
    }
}

void makeptr ( char* poem_arr, char** pointers, long number_of_strings)
{
    assert ( *poem_arr );
    assert ( poem_arr );
    assert ( pointers );
    assert ( isfinite (number_of_strings) );

    long i = 1;
    *pointers=poem_arr++;
    pointers++;
    char** temp_pointers = pointers;
    while ( i < number_of_strings)
    {
        if ( !(*poem_arr) )
        {

            *pointers = ++poem_arr;
            i++;
            if ( i!= number_of_strings ) pointers++;
        }
        else poem_arr++;
    }
    pointers = temp_pointers;
}

int strcmp ( const char* string1, const char* string2 )
{
    assert ( string1 );
    assert ( string2 );

    int i = 0;
    while ( string1[i] != '\0' && string2[i] != '\0' )
    {
        if ( lowercase_letter (string1[i]) != lowercase_letter (string2[i]) ) return lowercase_letter (string1[i]) - lowercase_letter (string2[i]);
        i++;
    }
    return lowercase_letter (string1[i]) - lowercase_letter (string2[i]);
}

void mySwap ( char** swap_array, long a, long b)
{
    assert (isfinite(a));
    assert (isfinite(b));
    assert (swap_array != NULL);

    char* temp = swap_array[a];
    swap_array[a] = swap_array[b];
    swap_array [b] = temp;
}

void quicksort (char** arr, long left, long right)
{
    assert (arr);
    assert ( isfinite (left) );
    assert ( isfinite (right) );

    if ( right <= left ) return;
    if ( right - left == 1 )
    {
        if ( strcmp( arr[left], arr[right]) > 0 )   mySwap ( arr, left, right );
        return;
    }
    long beginning = left;
    long ending = right;
    const char* mid = arr[ (left + right) / 2];
    while (left < right)
    {
        while ( strcmp( arr[left], mid ) < 0 ) left++;
        while ( strcmp( arr[right], mid) > 0 ) right--;
        if ( left < right )
        {
            mySwap (arr, left, right);
            left++;
            right--;
        }
    }
    quicksort ( arr, beginning, right);
    quicksort ( arr, left, ending);
}

char lowercase_letter (const char ch)
{
    if ( ch >= 'A' && ch <= 'Z' ) return ch + ('a' - 'A');
    return ch;
}

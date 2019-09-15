/// @file


// Log of changes:
//-----------------------------------------------------------------------------
// Version 1.1
// -Comporator doesn't change strings
//-----------------------------------------------------------------------------
// Version 1.2
// -Quicksort has new param: strcmp
//-----------------------------------------------------------------------------
// Version 1.3
// -Every element of pointer array has 2 pointers now:
// pointer to end of string (e_str) and pointer to beginning of string (b_str)
//-----------------------------------------------------------------------------
// Version 1.4
// Added reversed comporator, \0 in the beginnng of poem
//-----------------------------------------------------------------------------
// Version 1.5
// Added consts
//-----------------------------------------------------------------------------
// Version 1.6
// Mentor's requests were executed
//-----------------------------------------------------------------------------
// Version 1.7
// Included Unit-tests for comporators
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

struct pointer;

/// @brief Function calculates size of file
/// @param *stream Stream with file
/// @return Size of file
long  size_of_file ( FILE* stream);

/// @brief Function counts strings in array with text
/// @param *poem_arr Array containing text
/// @return Number of strings
long  stringcount ( char* poem_arr );

/// @brief Function assigns pointers to every string in array with text
/// @param *poem_arr Array containing text
/// @param **pointers Array with pointers
/// @param number_of_strings Number of strings
void  makeptr ( char* poem_arr, pointer* pointers, long number_of_strings);

/// @brief Compare 2 strings
/// @return Returns difference between first mismatching symbols in strings or returns 0 if first string if equal second one
int   direct_strcmp ( const pointer string1, const pointer string2 );

/// @brief Swap 2 strings in an array
/// @param mySwappingArray Array with strings
/// @param a Index of first string
/// @param b Index of second string
void  mySwap ( pointer* mySwappingArray, long a, long b);

/// @brief An usual quicksort, but for strings:)
/// @param **arr Array for sorting
/// @param left Lower edge for sort
/// @param right Upper edge for sort
void  quicksort ( pointer* arr, long left, long right, int (*strcmp) ( const pointer string1, const pointer string2 ));

/// @brief Opens file (You must write the name of file)
/// @return Stream with file
FILE* open_file ();

/// @brief Writes strings in file
/// @param **stringpointer Array containing pointers to strings
/// @param **number_of_strings Number of strings;
void write_in_file ( pointer* stringpointer, const long number_of_strings);


/// @param  [in] ch A symbol
/// @return ch if it is a lowercase letter or lowercase analogue of ch
char lowercase_letter (char ch);

/// @brief Compare 2 strings from its end ( Ignores symbol if it's not letter)
/// @return Returns difference between first mismatching symbols in strings or returns 0 if first string if equal second one
int reversed_strcmp ( const pointer string1, const pointer string2);

/// @brief Complex of tests for direct_strcmp
void test_direct_strcmp ();

/// @brief Complex of tests for reversed_strcmp
void test_reversed_strcmp ();

/// @brief Uses all the tests
void tests();

///@brief Struct containing 2 pointers: pointer to the beginning of string and pointer to the ending of the same string
struct pointer
{
    const char* b_ptr;
    const char* e_ptr;
};

const int FLEN = 300;

int main()
{
    tests();
    FILE* stream = open_file ();
    if (!stream) return 1;
    const long file_size = size_of_file ( stream );
    char* poem_arr = (char*) calloc ( file_size+2, sizeof(char) );
    if ( !poem_arr )
    {
        printf ("Memory can't be allocated\n");
        return 1;
    }
    *poem_arr = '\0';
    poem_arr++;
    const long number_of_symbols = fread ( poem_arr, sizeof(char), file_size, stream );
    poem_arr[number_of_symbols] = '\0';
    fclose (stream);
    const long number_of_strings = stringcount ( poem_arr );
    pointer* stringpointer = (pointer*)calloc ( number_of_strings, sizeof (pointer) );
    makeptr (poem_arr, stringpointer, number_of_strings);
    quicksort (stringpointer, 0, number_of_strings-1, direct_strcmp);
    printf ("Enter output file name for a normal sort: ");
    write_in_file ( stringpointer, number_of_strings);
    quicksort (stringpointer, 0, number_of_strings-1, reversed_strcmp);
    printf ("Enter output file name for a sort from the end: ");
    write_in_file ( stringpointer, number_of_strings);
    free(stringpointer);
    free(poem_arr);
    return 0;
}

FILE* open_file()
{
    FILE* stream;
    char INPUT_FILE_NAME[FLEN] = "";
    printf ("Enter input file name: ");
    scanf("%s", INPUT_FILE_NAME);
    if ( !( stream = fopen ( INPUT_FILE_NAME, "r") ) )
    {
        fprintf (stderr, "Input file is not open\n");
        return 0;
    }
    return stream;
}

void write_in_file( pointer* stringpointer, const long number_of_strings)
{
    assert (stringpointer);
    assert ( isfinite (number_of_strings) );

    FILE* stream;
    char OUTPUT_FILE_NAME[FLEN] = "";
    scanf ("%s", OUTPUT_FILE_NAME);
    if ( !( stream = fopen ( OUTPUT_FILE_NAME, "w") ) )
    {
        fprintf (stderr, "Input file is not open\n");
        return;
    }
    for ( int i = 0; i < number_of_strings; i++)
    {
        fprintf(stream, "%s\n", stringpointer->b_ptr );
        stringpointer++;
    }
    fclose (stream);
}

long size_of_file (FILE* stream)
{
    assert ( stream );

    fseek ( stream, 0, SEEK_END);
    const long filesize = ftell (stream);
    fseek ( stream, 0, SEEK_SET);
    return filesize;
}

long stringcount ( char* poem_arr )
{
    assert (  poem_arr );
    assert ( *poem_arr );

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

void makeptr ( char* poem_arr, pointer* pointers, long number_of_strings)
{
    assert ( poem_arr );
    assert ( *poem_arr );
    assert ( pointers );
    assert ( isfinite (number_of_strings) );

    long i = 1;
    pointers->b_ptr=poem_arr++;
    pointers++;
    while ( i < number_of_strings)
    {
        if ( !(*poem_arr) )
        {
            (pointers-1)->e_ptr = poem_arr - 1;
            pointers->b_ptr = poem_arr+1;
            i++;
            if ( i!= number_of_strings )
                {
                    pointers++;
                    poem_arr++;
                }
        }
        else poem_arr++;
    }
    while (*poem_arr) poem_arr++;
    pointers->e_ptr = poem_arr - 1;
}

void mySwap ( pointer* swap_array, long a, long b)
{
    assert (isfinite(a));
    assert (isfinite(b));
    assert (swap_array != NULL);

    pointer temp = swap_array[a];
    swap_array[a] = swap_array[b];
    swap_array [b] = temp;
}

void quicksort (pointer* arr, long left, long right, int (*strcmp) ( const pointer string1, const pointer string2 ))
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
    const long beginning = left;
    const long ending = right;
    const pointer mid = arr[ (left + right) / 2];
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
    quicksort ( arr, beginning, right, strcmp);
    quicksort ( arr, left, ending, strcmp);
}

char lowercase_letter (const char ch)
{
    if ( ch >= 'A' && ch <= 'Z' ) return ch + ('a' - 'A');
    return ch;
}

int direct_strcmp ( const pointer string1, const pointer string2 )
{
    assert ( string1.b_ptr );
    assert ( string2.b_ptr );

    int i = 0;
    while ( string1.b_ptr [i] != '\0' && string2.b_ptr [i] != '\0' )
    {
        if ( lowercase_letter (string1.b_ptr [i]) != lowercase_letter (string2.b_ptr[i]) ) return lowercase_letter (string1.b_ptr [i]) - lowercase_letter (string2.b_ptr [i]);
        i++;
    }
    return lowercase_letter (string1.b_ptr[i]) - lowercase_letter (string2.b_ptr[i]);
}

int reversed_strcmp ( const pointer string1, const pointer string2)
{
    assert ( string1.e_ptr );
    assert ( string2.e_ptr );

    int i = 0, j = 0;
    while ( string1.e_ptr [i] != '\0' && string2.e_ptr [j] != '\0' )
    {
        while ( (lowercase_letter (string1.e_ptr [i]) < 'a' || lowercase_letter (string1.e_ptr [i]) > 'z') && lowercase_letter (string1.e_ptr [i]) != '\0' ) i--;
        while ( (lowercase_letter (string2.e_ptr [j]) < 'a' || lowercase_letter (string2.e_ptr [j]) > 'z') && lowercase_letter (string2.e_ptr [j]) != '\0') j--;
        if ( lowercase_letter (string1.e_ptr [i]) != lowercase_letter (string2.e_ptr[j]) ) return lowercase_letter (string1.e_ptr [i]) - lowercase_letter (string2.e_ptr [j]);
        i--;
        j--;
    }
    return lowercase_letter (string1.e_ptr[i]) - lowercase_letter (string2.e_ptr[j]);
}



void tests()
{
    char wanna_test = 0;
    printf ("Wanna test? (Y/N): ");
    scanf ("%c", &wanna_test);
    printf ("\n");
    if ( wanna_test == 'Y' || wanna_test == 'y')
    {
        test_direct_strcmp ();
        test_reversed_strcmp ();
    }
}


void test_direct_strcmp1 ()
{
    const char* string1 = "abcd";
    const char* string2 = "bcde";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == -1 );
    assert( direct_strcmp (ptr2, ptr1) == 1 );
    printf ("first direct_strcmp test done!!!\n");
}

void test_direct_strcmp2 ()
{
    const char* string1 = "rdgs";
    const char* string2 = "r";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == 'd' );
    assert( direct_strcmp (ptr2, ptr1) == -'d' );
    printf ("second direct_strcmp test done!!!\n");
}

void test_direct_strcmp3 ()
{
    const char* string1 = "";
    const char* string2 = "f";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == -'f' );
    assert( direct_strcmp (ptr2, ptr1) == 'f' );
    printf ("third direct_strcmp test done!!!\n");
}

void test_direct_strcmp4 ()
{
    const char* string1 = "fesfesfesf";
    const char* string2 = "fesfesfesf";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == 0 );
    assert( direct_strcmp (ptr2, ptr1) == 0 );
    printf ("fourth direct_strcmp test done!!!\n");
}

void test_direct_strcmp ()
{
    test_direct_strcmp1 ();
    test_direct_strcmp2 ();
    test_direct_strcmp3 ();
    test_direct_strcmp4 ();
    printf ("DIRECT_STRCMP WORKS\n\n");
}




void test_reversed_strcmp1 ()
{
    const char* string1 = "abcd";
    const char* string2 = "abcc";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 1 );
    assert( reversed_strcmp (ptr2, ptr1) == -1 );
    printf ("first reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp2 ()
{
    const char* string1 = "abcccd";
    const char* string2 = "abcccd";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 0 );
    assert( reversed_strcmp (ptr2, ptr1) == 0 );
    printf ("second reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp3 ()
{
    const char* string1 = "a";
    const char* string2 = "red'";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == -3 );
    assert( reversed_strcmp (ptr2, ptr1) == 3 );
    printf ("third reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp4 ()
{
    const char* string1 = "aesgsrfs-3";
    const char* string2 = "asgfsfsdr65";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 1 );
    assert( reversed_strcmp (ptr2, ptr1) == -1 );
    printf ("first reversed_strcmp test done!!!\n");
}
void test_reversed_strcmp ()
{
    test_reversed_strcmp1 ();
    test_reversed_strcmp2 ();
    test_reversed_strcmp3 ();
    test_reversed_strcmp4 ();
    printf ("REVERSED_STRCMP WORKS\n\n");
}

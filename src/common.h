#ifndef _COMMON_H_
#define _COMMON_H_

#include <SDL.h>
#include <stdio.h>

#define DWORD unsigned long

// deinitialize everything and quit
void Quit();
void Quit(int code);

// double-value functions for minimum and maximum
double max(double i1, double i2);
double min(double i1, double i2);

/** startsWith()
 * return true if s2 starts with s1
 */
bool startsWith(const char* s1, const char* s2);



/** testCollision()
 * collision detection between 'second' and five points in 'first'
 * note that this only works if second can't skirt around 
 * inside of those points (e.g. if first is much bigger 
 * than the second)
 */
bool testCollision(SDL_Rect* first, SDL_Rect* second);



/** readLine()
 * reads len bytes from a line from a file
 * returns false if reached EOF, true to indicate more lines can be read
 */
bool readLine(FILE* f, char* buffer, int len);

/** strip()
 * remove prepending and appending whitespace
 * mallocs the returned string, be sure to free it
 */
char* strip(const char* s);

/** readFile()
 * Read the file contents of a file into a buffer. You must free the contents
 * with 'delete' when you are finished. Returns NULL and prints out a warning
 * to STDERR on failure.
 */
char* readFile(const char* fname);

/** hash()
 * gives a unique integer that can be used to quickely compare strings
 */
uint32_t hash(const char* s);


#endif // _COMMON_H_


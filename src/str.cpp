#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "str.h"

#ifndef ltoa
void ltoa(int i, char* buf, int)
{
	sprintf(buf, "%d", i);
}
#endif

Str::Str(int len) : data(NULL), len(0), alloc(0) {
	buffer(len);
}

Str::~Str() {
	free();
}

int Str::length() {
	return len;
}

void Str::buffer(int sz) {
	if (len == 0) {
		free();
		data = new char[sz];
		alloc = sz;
		return;
	}

	// TODO: make sure newData can hold data
	int l = len;
	char* newData = new char[sz];
	strcpy(newData, data);

	free();
	len = l;
	alloc = sz;
	data = newData;
}

inline void Str::expand(int sz) {
	while (sz > alloc - len - 1)
		buffer(alloc * 3 + 1);
}

void Str::free() {
	if (data == NULL)
		return;
	delete data;
	data = NULL;
	len = alloc = 0;
}

char* Str::copy() {
	if (!data)
		return NULL;
	char* s = new char[len + 1];

	strcpy(s, data);

	return s;
}

Str::operator const char*() {
	return data;
}

Str& Str::operator +=(const char* str) {
	if (!str || str[0] == '\0')
		return *this;

	int l = strlen(str);
	expand(l);
	strcpy(&data[len], str);
	len += l;
	data[len] = 0;
	return *this;
}

Str& Str::operator +=(bool b) {
	return b ? operator +=("true") : operator +=("false");
}

Str& Str::operator +=(char c) {
	expand(1);
	data[len++] = c;
	data[len] = 0;
	return *this;
}

Str& Str::operator +=(short i) {
	char buf[64];
	ltoa(i, buf, 10);
	return operator +=(buf);
}

Str& Str::operator +=(int i) {
	char buf[64];
	ltoa(i, buf, 10);
	return operator +=(buf);
}

Str& Str::operator +=(long i) {
	char buf[64];
	ltoa(i, buf, 10);
	return operator +=(buf);
}

#if 0
Str& Str::operator +=(long long i) {
	char buf[64];
	ltoa(i, buf, 10);
	return operator +=(buf);
}
#endif

bool Str::operator ==(const char* str) {
	if (!str)
		return !data;
	return !strcmp(data, str);
}

bool Str::operator !=(const char* str) {
	if (!str)
		return !data;
	return strcmp(data, str);
}


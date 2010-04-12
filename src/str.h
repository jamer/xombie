#ifndef _STR_H_
#define _STR_H_

class Str {
public:

	// Constructors
//	Str();
	Str(int len);
//	Str(const char* str);
//	Str(Str& str);

	// Deconstructor
	~Str();

	// Methods
	int length();
	void buffer(int len);
	void freeStr();
	char* copy();

	operator const char*();

	// Operators
//	char& operator [](int index);

//	Str& operator =(const char* str);
//	Str& operator =(Str& str);

	Str& operator +=(const char* str);

	Str& operator +=(bool b);
	Str& operator +=(char c);
	Str& operator +=(short i);
	Str& operator +=(int i);
	Str& operator +=(long i);
	Str& operator +=(unsigned long i);
//	Str& operator +=(long long i);

	bool operator ==(const char* str);
//	bool operator ==(Str& str);

	bool operator !=(const char* str);
//	bool operator !=(Str& str);

//	bool operator !();

private:
	inline void expand(int len);

	// Members
	char* data;
	int len;
	int alloc;
};

#endif // _STR_H_


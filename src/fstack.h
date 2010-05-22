#ifndef _FSTACK_H_
#define _FSTACK_H_

#include <stdlib.h>

/* Fstack, a simple stack implementation using templates. Its name comes from
 * the fact that it's a "Fast stack."
 *
 * If using this class with any sizable data type, using pointers to data
 * instead of raw data itself may be faster.
 */
template<class T> class Fstack
{
public:
	/**
	 * Create a stack, specifying the initial allocated size. The stack
	 * will dynamically resize when you push enough elements onto it, but
	 * doing so will use CPU cycles and requires a call to 
	 */
	Fstack(int initbufsz);


	/**
	 * Pushes an element onto the stack. If the stack does not have enough
	 * memory to hold this latest addition, it will double its allocated
	 * memory before pushing.
	 */
	void push(T t);

	/**
	 * Pops an element off the stack.
	 */
	T pop();


	/**
	 * Peek at the top element in the stack without popping it off.
	 */
	T top();

	/**
	 * Grab any element from the stack. Index [0] refers to the first
	 * element pushed onto the stack. Index [size() - 1] refers to the
	 * latest element pushed onto the stack.
	 */
	T index(int i);

	/**
	 * Returns the number of elements in the stack currently.
	 */
	int size();

private:
	T* data; // The stack
	unsigned int sz; // Items in stack
	unsigned int total; // Allocated size of stack
};


template<class T> Fstack<T>::Fstack(int initbufsz)
{
	total = initbufsz;
	sz = 0;
	data = (T*)malloc(sizeof(T) * total);
}

template<class T> void Fstack<T>::push(T t)
{
	if (sz == total) {
		total *= 2;
		data = (T*)realloc(data, sizeof(T) * total * 2);
	}
	data[sz++] = t;
}

template<class T> T Fstack<T>::pop()
{
	if (sz == 0)
		return NULL;
	return data[(sz--) - 1];
}

template<class T> T Fstack<T>::top()
{
	if (sz == 0)
		return NULL;
	return data[sz - 1];
}

template<class T> T Fstack<T>::index(int i)
{
	if (i == 0 || i >= sz)
		return NULL;
	return data[i];
}

template<class T> int Fstack<T>::size()
{
	return sz;
}


#endif // _FSTACK_H_


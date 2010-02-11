
#include "fstack.h"
#include "view.h"

template Fstack<View*>::Fstack(int initbufsz);
template View* Fstack<View*>::top();
template void Fstack<View*>::push(View* t);


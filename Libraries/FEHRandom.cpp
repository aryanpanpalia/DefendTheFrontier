#include "FEHRandom.h"
#include "FEHUtility.h"

#include <time.h>

FEHRandom Random;

void FEHRandom::Initialize()
{
	// This function is only present to make the code compile and match the header file
}

void FEHRandom::Seed()
{
	srand(time(NULL));
}

int FEHRandom::RandInt()
{
	return rand(); // steal rand from stdlib.h
}
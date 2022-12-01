#pragma once

#include <stdlib.h>
#include <FEHLCD.h>
#include <fstream>
#include <iostream>

class Image
{
	public:
    	int rows, cols;
		int *saved_image;
		Image();
		void Open(const char *);
		void Draw(int, int);
		void Close();

};
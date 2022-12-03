#pragma once

#include <stdlib.h>
#include <FEHLCD.h>
#include <fstream>
#include <iostream>

/*
    Image Class: 

    Contains variables that stores:
        rows: number of rows in the iamge
		cols: number of cols in the iamge
		saved_image: array containing pixel values of image

    Contains methods that:
        Image(): creates the image object
		Open(const char*): opens an image
		Draw(int, int): draws an image to the screen
		Close(): closes an image
*/
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

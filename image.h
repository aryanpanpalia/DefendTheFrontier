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
		Rotate180(): rotates image 180 degrees
		Close(): closes an image

	Authors: Ben Grier, Clayton Greenbaum, Aryan Panpalia
*/
class Image
{
	public:
    	int rows, cols;
		int *saved_image;
		Image();
		void Open(const char *);
		void Draw(int, int);
		void Rotate180();
		void Close();
};

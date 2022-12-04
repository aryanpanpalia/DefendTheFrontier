// Code by Ben Grier with help from Clayton Greenbaum
// Code modified by Aryan Panpalia
#include <image.h>

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

/*
	Creates an Image object

	Parameters: none
	Return value: none

	Authors: Ben Grier, Clayton Greenbaum
*/
Image::Image()
{
	rows = 0;
	cols = 0;
	saved_image = NULL;
}

/*
	Opens an image

	Parameters: 
		filename: file output by MATLAB to draw that ends in a .pic

	Return value: none

	Authors: Ben Grier, Clayton Greenbaum
*/
void Image::Open(const char *filename)
{
	// Create file stream
	std::ifstream pic;
	pic.open(filename);

	//MATLAB outputs picture files in a rows by cols format
	//User interface is completely in an x,y format
	if (pic.is_open())
	{
		pic >> rows >> cols;
	}
	else 
		std::cout << "File: " << filename << " did not open!\n";

	// If an image was already loaded, free the memory before opening new image
	if (saved_image != NULL)
	{
		free(saved_image);
		saved_image = NULL;
	}

	// Allocate memory for new image
	saved_image = (int*)malloc(sizeof(int) * rows * cols);

	// Read image from data file
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols && !pic.eof(); j++)
		{
			pic >> *(saved_image + (i * cols) + j);
		}
	}

	if (!pic.fail()) {
		pic.close();
	}
}

/*
	Draws the image to the screen

	Parameters:
		x: x position of top left corner of where the draw the picture
		y: y position of top left corner of where the draw the picture

	Authors: Ben Grier, Clayton Greenbaum, Aryan Panpalia
*/
void Image::Draw(int x, int y)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (*(saved_image + (i * cols) + j) != -1)
			{
				// Only draw the pixel if it is within the bounds of the window
				if(0 <= j + x && j + x <= WINDOW_WIDTH && 0 <= i + y && i + y <= WINDOW_HEIGHT) {
					LCD.SetFontColor(*(saved_image + (i * cols) + j));
					LCD.DrawPixel(j + x, i + y);
				}
			}
		}
	}
}

/*
	Rotates image 180 degrees

	Parameters: none
	Return value: none

	Authors: Aryan Panpalia
*/
void Image::Rotate180() {
	int *newImage = (int*)malloc(sizeof(int) * rows * cols);

	for (int oldRow = 0; oldRow < rows; oldRow++)
	{
		for (int oldCol = 0; oldCol < cols; oldCol++)
		{
			int newRow = rows - oldRow - 1;
			int newCol = cols - oldCol - 1;
			newImage[newRow * cols + newCol] = saved_image[oldRow * cols + oldCol];
		}
	}

	if (saved_image != NULL) {
		free(saved_image);
	}
	saved_image = newImage;
}

/*
	Closes an image and prevents memory leak issues after malloc

	Parameters: none
	Return value: none

	Authors: Ben Grier, Clayton Greenbaum, Thomas Banko
*/
void Image::Close()
{
	if (saved_image != NULL) {
		free(saved_image);
	}
}

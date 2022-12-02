// Code by Ben Grier with help from Clayton Greenbaum
// Code modified by Aryan Panpalia
#include <Image.h>

Image::Image()
{
	rows = 0;
	cols = 0;
	saved_image = NULL;
}

//filename is file output by MATLAB to draw. Should end in *FEH.pic
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

//x,y are top left location of where to draw picture
void Image::Draw(int x, int y)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (*(saved_image + (i * cols) + j) != -1)
			{
				LCD.SetFontColor(*(saved_image + (i * cols) + j));
				LCD.DrawPixel(j + x, i + y);
			}
		}
	}
}

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

//prevent memory leak issues after malloc
void Image::Close()
{
	if (saved_image != NULL) {
		free(saved_image);
	}
}

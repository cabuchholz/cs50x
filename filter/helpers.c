#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE p;
            BYTE b;
            b = round(((double) image[i][j].rgbtRed + (double) image[i][j].rgbtGreen + (double) image[i][j].rgbtBlue)/3);

            //Assign to temporary variable
            p.rgbtRed = b;
            p.rgbtBlue = b;
            p.rgbtGreen = b;

            //Insert back into original image
            *&image[i][j].rgbtRed = p.rgbtRed;
            *&image[i][j].rgbtBlue = p.rgbtBlue;
            *&image[i][j].rgbtGreen = p.rgbtGreen;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
   for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Calculate raw values
            long rred = round(.393 * (double) image[i][j].rgbtRed + .769 * (double) image[i][j].rgbtGreen + .189 * (double) image[i][j].rgbtBlue);
            long rgreen = round(.349 * (double) image[i][j].rgbtRed + .686 * (double) image[i][j].rgbtGreen + .168 * (double) image[i][j].rgbtBlue);
            long rblue = round(.272 * (double) image[i][j].rgbtRed + .534 * (double) image[i][j].rgbtGreen + .131 * (double) image[i][j].rgbtBlue);

            //Ensure that values 255 or below
            if (rred > 255)
            {
                *&image[i][j].rgbtRed = 255;
            }
            else
            {
                *&image[i][j].rgbtRed = rred;
            }

            if (rgreen > 255)
            {
                *&image[i][j].rgbtGreen = 255;
            }
            else
            {
                *&image[i][j].rgbtGreen = rgreen;
            }

            if (rblue > 255)
            {
                *&image[i][j].rgbtBlue = 255;
            }
            else
            {
                *&image[i][j].rgbtBlue = rblue;
            }

        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Create new placeholder image, to enable reflection
    RGBTRIPLE(*image2)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    //Iterate over original image and input into placeholder image with reflection
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image2[i][j] = image[i][width - 1 - j];
        }
    }

    //Iterate over replacement image, and input back into the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *&image[i][j] = image2[i][j];
        }
    }

    // Free replacement memory for image
    free(image2);

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create new placeholder image, to enable reflection
    RGBTRIPLE(*image2)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    //Create offset to blur by 1 over the height and width
    int delta[3] = {-1, 0, 1};

    //Loop over all elements in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Create a counter to track number of adjacent cells in the bounds, as well as the RGB sum values
            int counter = 0;
            long sred = 0;
            long sgreen = 0;
            long sblue = 0;

            //Iterate through all of the possible adjacent cells
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    //Check if within the bounds of image
                    if ((i + delta[x] >= 0 && i + delta[x] < height) && (j + delta[y] >= 0 && j + delta[y] < width))
                    {
                        //Add to counter and to sum of colors
                        counter++;
                        sred = sred + image[i + delta[x]][j + delta[y]].rgbtRed;
                        sgreen = sgreen + image[i + delta[x]][j + delta[y]].rgbtGreen;
                        sblue = sblue + image[i + delta[x]][j + delta[y]].rgbtBlue;
                    }
                }
            }

            //Take the average value of the RGB and place-into placeholder image
            image2[i][j].rgbtRed = round((double) sred/ (float) counter);
            image2[i][j].rgbtGreen = round((double) sgreen/ (float) counter);
            image2[i][j].rgbtBlue = round((double) sblue/ (float) counter);

        }
    }

    //Iterate over replacement image, and input back into the original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *&image[i][j] = image2[i][j];
        }
    }

    // Free replacement memory for image
    free(image2);

    return;
}

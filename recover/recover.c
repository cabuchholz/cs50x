#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Define new data type for Byte
typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
//Check to ensure that only one command line arguement was given
if (argc != 2)
{
    fprintf(stderr, "Single file not specified\n");
        return 1;
}

//Create pointer to file to open
FILE *f = fopen(argv[1], "r");

//Check to ensure an actual file name was given
if (f == NULL)
{
    fprintf(stderr, "Not passed readable file\n");
        return 1;
}

//Print the address Open is pointing to
//printf("%p %p\n", f, f + sizeof(BYTE));
//printf("%p %p\n", f, f + 1);

//Create counter for location of file reading from
//long loc = 0;

//Create variable to store size of blocks
int block = 512;

//Create variable to store the JPEG
int jpg = 0;

//Run a loop reading over the file, until reach the end
int loop = 2;

//Create a buffer to store the data
BYTE* buffer = calloc(block, sizeof(BYTE));

//Declare an image file to fill
FILE *img;

//Loop as long as chunk being read is 512 bytes long
while(fread(buffer, sizeof(BYTE), block, f) >= block)
{
    //Check if start of JPEG
    if (buffer[0] == 0xff && buffer[1] == 0xd8  && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {

        //Create a string to hold new file name
        char *filename = calloc(8, sizeof(char));

        //Name it based upon number of jpg found
        sprintf(filename, "%03i.jpg", jpg);

        //Create a file to append to
        img = fopen(filename, "w");

        //Append to the file
        fwrite(buffer, sizeof(BYTE), block, img);

        //Increment on the jpg number
        jpg = jpg + 1;
    }
    else if (jpg > 0)
        {
            //Append to the file
            fwrite(buffer, sizeof(BYTE), block, img);
        }
}
}
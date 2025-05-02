//------------------------------------------------------------------------------------------------
// File:        main.cpp
// Description: Main entry point for the Pisco Code library.
// Author:      Andre Viegas
// Date:        2025.04
// Version:     1.0
//------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf("Hello, Pisco Code!\n\n");

    // Get the number to be shown in the LED
    int number = 0;
    if (argc > 1)
    {
        number = atoi(argv[1]);
    }
    else
    {
        printf("Please provide a number as an argument.\n");
        return 1;
    }

    printf("The number is: %d\n", number);
    return 0;
}

#ifndef USEFUL_FUNC_H
#define USEFUL_FUNC_H
#include <ctype.h>
#include <stdio.h>

bool isNumber(char number[])
{
    int i = 0;
    //checking for negative numbers
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
		{
			printf("invalid input , except positive int \n");
            return false;
		}
    }
    return true;
}

#endif

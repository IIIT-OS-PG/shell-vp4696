#include<iostream>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>

using namespace std;


int parser(char* t,char* temp[])
{
	char *par;
	int i=0;

	par = t;
	temp[i] = strtok(par, " \n");
	do 
	{
		i++;
		par = NULL;
		temp[i] = strtok(par, " \n"); 
	}while(temp[i] != NULL) ;

	return i;
}	

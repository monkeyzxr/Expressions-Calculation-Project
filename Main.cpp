//*******************************************************
// Class:       COSC 2336.S01
// Assignment : Project
// Author:      Xiangru Zhou
// Date:        12-01-2015
//*******************************************************

#include"Statement.h"
#include<iostream>

using namespace std;

void main (int argc, char* argv[])
    {
	Statement   MyST;
	WCS_String  Temp   (argv[1]);
	MyST.ReadStatement (Temp);
    }
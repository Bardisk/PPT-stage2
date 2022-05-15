#include<cstdio>

#ifndef _COLOR_H_
#define _COLOR_H_

//#define setcol(hOP,a) SetConsoleTextAttribute(hOP,a)

#define Blue 0x04
#define Green 0x02
#define Red 0x01
#define Yellow 0x03
#define Purple 0x05
#define Cyan 0x06
#define White 0x07
#define Back 40
#define Fore 30
#define Intense 60
void setcol(int col);
#define rescol() printf("\033[0m")
#define setIntense() printf("\033[1m")

#endif

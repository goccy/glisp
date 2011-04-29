#include "greadline.h"

//===========================MAIN INTERFACE================================//

char *greadline(char *init_message)
{
	struct termio tty_backup; //create backup
	ioctl(0, TCGETA, &tty_backup); //save current status
	changeTermStatus(tty_backup);//set non-canonical mode
	char *line = readBuf(tty_backup, init_message);//get line
	ioctl(0, TCSETAF, &tty_backup);//replace prev status
	CHANGE_COLOR(WHITE);
	fprintf(stderr, "\n");
	return line;
}

//=========================================================================//

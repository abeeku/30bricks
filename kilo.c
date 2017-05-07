/** Clude **/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
/** Clude **/

/** storage **/
struct termios orig_termios;
/** storage  **/


void crashingthisplane(const char *s){
  perror(s);
  exit(1);
}

void disableRawMode(){
 if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
   crashingthisplane("tcsetattr");
}
void enableRawMode() {

 if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) crashingthisplane("tcsetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON| IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) crashingthisplane("tcsetattr");
}
int main() {
  enableRawMode();

  while (1){
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) crashingthisplane ("read my lips");
    if(iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n",c,c);
    }
    if (c == 'q') break;
  }
  return 0;
}


#include <stdio.h>
#include <unistd.h>

int main() {
	char ch;
	printf("CHANDLER'S ROT13 [EN|DE]CODER\n");
	while (read(STDIN_FILENO, &ch, 1) > 0) {
		if ((ch >= 'A' && ch <= 'M') || (ch >= 'a' && ch <= 'm')) {
			printf("%c", ch+13);
		} else if ((ch >= 'N' && ch <= 'Z') || (ch >= 'n' && ch <= 'z')) {
			printf("%c", ch-13);
		} else {
			printf("%c", ch);
		}
	}
}

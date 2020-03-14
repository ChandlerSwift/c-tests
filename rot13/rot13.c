#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc > 1) {
		printf("Usage: simply pipe text to stdin. For example:\n\n  cat text | ./rot13\n");
		return 1;
	}
	char ch;
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

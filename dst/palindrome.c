#include <stdio.h>
int is_palindrome(int n) {
	char str[6] = {0};
	int i = 0;
	while (n > 0)
	{
		str[i++] = n % 10;
		n /= 10;
	}
	for (int j = 0; j < i / 2; j++)
	{
		if (str[j] != str[i - j - 1])
		{
			return 0;
		}
	}
	return 1;
}

int main() {
	int n;
	scanf("%d", &n);

	if (is_palindrome(n)) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}

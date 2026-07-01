#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "libasm.h"

static int g_tests = 0;
static int g_pass = 0;

static void check(const char *name, int cond)
{
	g_tests++;
	if (cond)
	{
		g_pass++;
		printf("  [OK] %s\n", name);
	}
	else
		printf("  [KO] %s\n", name);
}

static void test_strlen(void)
{
	printf("\n=== ft_strlen ===\n");
	check("empty string", ft_strlen("") == strlen(""));
	check("hello", ft_strlen("hello") == strlen("hello"));
	check("longer string", ft_strlen("Hello, World!") == strlen("Hello, World!"));
	check("single char", ft_strlen("a") == strlen("a"));
}

static void test_strcpy(void)
{
	char buf1[64];
	char buf2[64];

	printf("\n=== ft_strcpy ===\n");
	check("hello (return)", ft_strcpy(buf1, "hello") == buf1);
	strcpy(buf2, "hello");
	check("hello (content)", strcmp(buf1, buf2) == 0);
	ft_strcpy(buf1, "");
	check("empty string", strcmp(buf1, "") == 0);
	ft_strcpy(buf1, "42 Tokyo");
	check("42 Tokyo", strcmp(buf1, "42 Tokyo") == 0);
}

static void test_strcmp(void)
{
	printf("\n=== ft_strcmp ===\n");
	check("equal strings", ft_strcmp("hello", "hello") == 0);
	check("empty strings", ft_strcmp("", "") == 0);
	check("s1 < s2 (sign)", ft_strcmp("abc", "abd") < 0);
	check("s1 > s2 (sign)", ft_strcmp("abd", "abc") > 0);
	check("s1 shorter", ft_strcmp("ab", "abc") < 0);
	check("s1 longer", ft_strcmp("abc", "ab") > 0);
}

static void test_write(void)
{
	ssize_t ret1;
	ssize_t ret2;

	printf("\n=== ft_write ===\n");
	ret1 = ft_write(1, "ft_write test\n", 14);
	ret2 = write(1, "write test\n", 11);
	check("write to stdout (ret)", ret1 == 14 && ret2 == 11);

	errno = 0;
	ret1 = ft_write(-1, "bad", 3);
	check("bad fd returns -1", ret1 == -1);
	check("bad fd sets errno EBADF", errno == EBADF);
}

static void test_read(void)
{
	int fd;
	char buf[64];
	ssize_t ret;

	printf("\n=== ft_read ===\n");
	fd = open("/etc/hostname", O_RDONLY);
	if (fd >= 0)
	{
		memset(buf, 0, sizeof(buf));
		ret = ft_read(fd, buf, 63);
		check("read from file (ret > 0)", ret > 0);
		check("read from file (content)", strlen(buf) > 0);
		close(fd);
	}
	else
	{
		fd = open("/etc/os-release", O_RDONLY);
		if (fd >= 0)
		{
			memset(buf, 0, sizeof(buf));
			ret = ft_read(fd, buf, 63);
			check("read from file (ret > 0)", ret > 0);
			check("read from file (content)", strlen(buf) > 0);
			close(fd);
		}
		else
			printf("  [SKIP] no readable file found\n");
	}

	errno = 0;
	ret = ft_read(-1, buf, 10);
	check("bad fd returns -1", ret == -1);
	check("bad fd sets errno EBADF", errno == EBADF);
}

static void test_strdup(void)
{
	char *dup;

	printf("\n=== ft_strdup ===\n");
	dup = ft_strdup("hello");
	check("hello (content)", strcmp(dup, "hello") == 0);
	free(dup);

	dup = ft_strdup("");
	check("empty string", strcmp(dup, "") == 0);
	free(dup);

	dup = ft_strdup("42 Tokyo libasm");
	check("longer string", strcmp(dup, "42 Tokyo libasm") == 0);
	free(dup);
}

int main(void)
{
	test_strlen();
	test_strcpy();
	test_strcmp();
	test_write();
	test_read();
	test_strdup();
	printf("\n=== Results: %d/%d passed ===\n\n", g_pass, g_tests);
	return (g_pass == g_tests) ? 0 : 1;
}

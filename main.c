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

static char *make_long_string(size_t len)
{
	char *s = malloc(len + 1);
	if (!s)
		return NULL;
	memset(s, 'A', len);
	s[len] = '\0';
	return s;
}

/* 評価シート: empty string, very long string */
static void test_strlen(void)
{
	char *longstr;

	printf("\n=== ft_strlen ===\n");
	check("empty string", ft_strlen("") == 0);
	check("hello", ft_strlen("hello") == 5);
	check("single char", ft_strlen("a") == 1);
	longstr = make_long_string(10000);
	if (longstr)
	{
		check("very long string (10000)", ft_strlen(longstr) == 10000);
		free(longstr);
	}
	check("NULL returns 0", ft_strlen(NULL) == 0);
}

/* 評価シート: empty string, very long string */
static void test_strcpy(void)
{
	char buf[128];
	char *longstr;
	char *longbuf;

	printf("\n=== ft_strcpy ===\n");
	check("return value", ft_strcpy(buf, "hello") == buf);
	check("hello", strcmp(buf, "hello") == 0);
	ft_strcpy(buf, "");
	check("empty string", strcmp(buf, "") == 0);
	ft_strcpy(buf, "42 Tokyo");
	check("42 Tokyo", strcmp(buf, "42 Tokyo") == 0);
	check("NULL src returns dst", ft_strcpy(buf, NULL) == buf);
	longstr = make_long_string(10000);
	longbuf = malloc(10001);
	if (longstr && longbuf)
	{
		ft_strcpy(longbuf, longstr);
		check("very long string (10000)", strcmp(longbuf, longstr) == 0);
	}
	free(longstr);
	free(longbuf);
}

/* 評価シート: 2 empty strings, 1 empty as first, 1 empty as second,
   multiple strings equal or not, switching them */
static void test_strcmp(void)
{
	printf("\n=== ft_strcmp ===\n");
	check("2 empty strings", ft_strcmp("", "") == 0);
	check("empty as first arg", ft_strcmp("", "hello") < 0);
	check("empty as second arg", ft_strcmp("hello", "") > 0);
	check("equal strings", ft_strcmp("hello", "hello") == 0);
	check("s1 < s2", ft_strcmp("abc", "abd") < 0);
	check("s1 > s2", ft_strcmp("abd", "abc") > 0);
	check("switched: s2 < s1", ft_strcmp("abd", "abc") > 0);
	check("switched: s1 < s2", ft_strcmp("abc", "abd") < 0);
	check("s1 shorter", ft_strcmp("ab", "abc") < 0);
	check("s1 longer", ft_strcmp("abc", "ab") > 0);
	check("single char equal", ft_strcmp("a", "a") == 0);
	check("single char diff", ft_strcmp("a", "b") < 0);
	check("NULL == NULL", ft_strcmp(NULL, NULL) == 0);
	check("NULL < non-NULL", ft_strcmp(NULL, "x") < 0);
	check("non-NULL > NULL", ft_strcmp("x", NULL) > 0);
}

/* 評価シート: stdout, open fd, wrong fd, return value, errno check */
static void test_write(void)
{
	ssize_t ret;
	int fd;

	printf("\n=== ft_write ===\n");

	/* stdout */
	ret = ft_write(1, "ft_write to stdout\n", 19);
	check("stdout return value", ret == 19);

	/* open file descriptor */
	fd = open("/tmp/libasm_test_write.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
	{
		ret = ft_write(fd, "file test", 9);
		check("open fd return value", ret == 9);
		close(fd);
		unlink("/tmp/libasm_test_write.tmp");
	}

	/* wrong file descriptor */
	errno = 0;
	ret = ft_write(-1, "bad", 3);
	check("wrong fd returns -1", ret == -1);
	check("wrong fd sets errno EBADF", errno == EBADF);

	/* write 0 bytes */
	ret = ft_write(1, "", 0);
	check("write 0 bytes returns 0", ret == 0);
}

/* 評価シート: stdin(skip in auto), open fd, wrong fd, return value, errno check */
static void test_read(void)
{
	int fd;
	int write_fd;
	char buf[128];
	ssize_t ret;

	printf("\n=== ft_read ===\n");

	/* open file descriptor */
	write_fd = open("/tmp/libasm_test_read.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (write_fd >= 0)
	{
		write(write_fd, "read test data", 14);
		close(write_fd);
		fd = open("/tmp/libasm_test_read.tmp", O_RDONLY);
		if (fd >= 0)
		{
			memset(buf, 0, sizeof(buf));
			ret = ft_read(fd, buf, 127);
			check("open fd return value", ret == 14);
			check("open fd content", strcmp(buf, "read test data") == 0);
			close(fd);
		}
		unlink("/tmp/libasm_test_read.tmp");
	}

	/* wrong file descriptor */
	errno = 0;
	ret = ft_read(-1, buf, 10);
	check("wrong fd returns -1", ret == -1);
	check("wrong fd sets errno EBADF", errno == EBADF);

	/* read 0 bytes */
	fd = open("/dev/null", O_RDONLY);
	if (fd >= 0)
	{
		ret = ft_read(fd, buf, 0);
		check("read 0 bytes returns 0", ret == 0);
		close(fd);
	}
}

/* 評価シート: empty string, very long string */
static void test_strdup(void)
{
	char *dup;
	char *longstr;

	printf("\n=== ft_strdup ===\n");

	dup = ft_strdup("");
	check("empty string", dup != NULL && strcmp(dup, "") == 0);
	free(dup);

	dup = ft_strdup("hello");
	check("hello", dup != NULL && strcmp(dup, "hello") == 0);
	free(dup);

	dup = ft_strdup("42 Tokyo libasm");
	check("normal string", dup != NULL && strcmp(dup, "42 Tokyo libasm") == 0);
	free(dup);

	longstr = make_long_string(10000);
	if (longstr)
	{
		dup = ft_strdup(longstr);
		check("very long string (10000)", dup != NULL && strcmp(dup, longstr) == 0);
		check("strdup is independent copy", dup != longstr);
		free(dup);
		free(longstr);
	}

	check("NULL returns NULL", ft_strdup(NULL) == NULL);
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

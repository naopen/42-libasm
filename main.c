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
	check("empty string", ft_strlen("") == strlen(""));
	check("hello", ft_strlen("hello") == strlen("hello"));
	check("single char", ft_strlen("a") == strlen("a"));
	check("with spaces", ft_strlen("hello world") == strlen("hello world"));
	longstr = make_long_string(10000);
	if (longstr)
	{
		check("very long string (10000)", ft_strlen(longstr) == strlen(longstr));
		free(longstr);
	}
	check("NULL returns 0", ft_strlen(NULL) == 0);
}

/* 評価シート: empty string, very long string */
static void test_strcpy(void)
{
	char buf1[128];
	char buf2[128];
	char *longstr;
	char *longbuf1;
	char *longbuf2;

	printf("\n=== ft_strcpy ===\n");
	check("return value", ft_strcpy(buf1, "hello") == buf1);
	strcpy(buf2, "hello");
	check("hello", strcmp(buf1, buf2) == 0);
	ft_strcpy(buf1, "");
	strcpy(buf2, "");
	check("empty string", strcmp(buf1, buf2) == 0);
	ft_strcpy(buf1, "42 Tokyo");
	strcpy(buf2, "42 Tokyo");
	check("42 Tokyo", strcmp(buf1, buf2) == 0);
	check("NULL src returns dst", ft_strcpy(buf1, NULL) == buf1);
	longstr = make_long_string(10000);
	longbuf1 = malloc(10001);
	longbuf2 = malloc(10001);
	if (longstr && longbuf1 && longbuf2)
	{
		ft_strcpy(longbuf1, longstr);
		strcpy(longbuf2, longstr);
		check("very long string (10000)", strcmp(longbuf1, longbuf2) == 0);
	}
	free(longstr);
	free(longbuf1);
	free(longbuf2);
}

/* 評価シート: 2 empty strings, 1 empty as first, 1 empty as second,
   multiple strings equal or not, switching them */
static void test_strcmp(void)
{
	printf("\n=== ft_strcmp ===\n");
	check("2 empty strings",
		ft_strcmp("", "") == strcmp("", ""));
	check("empty as first arg",
		(ft_strcmp("", "hello") < 0) == (strcmp("", "hello") < 0));
	check("empty as second arg",
		(ft_strcmp("hello", "") > 0) == (strcmp("hello", "") > 0));
	check("equal strings",
		ft_strcmp("hello", "hello") == strcmp("hello", "hello"));
	check("s1 < s2",
		(ft_strcmp("abc", "abd") < 0) == (strcmp("abc", "abd") < 0));
	check("s1 > s2",
		(ft_strcmp("abd", "abc") > 0) == (strcmp("abd", "abc") > 0));
	check("switched: s2 < s1",
		(ft_strcmp("abd", "abc") > 0) == (strcmp("abd", "abc") > 0));
	check("switched: s1 < s2",
		(ft_strcmp("abc", "abd") < 0) == (strcmp("abc", "abd") < 0));
	check("s1 shorter",
		(ft_strcmp("ab", "abc") < 0) == (strcmp("ab", "abc") < 0));
	check("s1 longer",
		(ft_strcmp("abc", "ab") > 0) == (strcmp("abc", "ab") > 0));
	check("single char equal",
		ft_strcmp("a", "a") == strcmp("a", "a"));
	check("single char diff",
		(ft_strcmp("a", "b") < 0) == (strcmp("a", "b") < 0));
	check("NULL == NULL", ft_strcmp(NULL, NULL) == 0);
	check("NULL < non-NULL", ft_strcmp(NULL, "x") < 0);
	check("non-NULL > NULL", ft_strcmp("x", NULL) > 0);
}

/* 評価シート: stdout, open fd, wrong fd, return value, errno check */
static void test_write(void)
{
	ssize_t ft_ret;
	ssize_t libc_ret;
	int fd1;
	int fd2;

	printf("\n=== ft_write ===\n");

	/* stdout */
	ft_ret = ft_write(1, "ft_write to stdout\n", 19);
	libc_ret = write(1, "write to stdout\n", 16);
	check("stdout return value", ft_ret == 19 && libc_ret == 16);

	/* open file descriptor */
	fd1 = open("/tmp/libasm_test_ft.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	fd2 = open("/tmp/libasm_test_libc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd1 >= 0 && fd2 >= 0)
	{
		ft_ret = ft_write(fd1, "file test", 9);
		libc_ret = write(fd2, "file test", 9);
		check("open fd return value", ft_ret == libc_ret);
		close(fd1);
		close(fd2);
		unlink("/tmp/libasm_test_ft.tmp");
		unlink("/tmp/libasm_test_libc.tmp");
	}

	/* wrong file descriptor */
	errno = 0;
	ft_ret = ft_write(-1, "bad", 3);
	int ft_errno = errno;
	errno = 0;
	libc_ret = write(-1, "bad", 3);
	int libc_errno = errno;
	check("wrong fd returns -1", ft_ret == libc_ret);
	check("wrong fd sets errno EBADF", ft_errno == libc_errno);

	/* write 0 bytes */
	ft_ret = ft_write(1, "", 0);
	libc_ret = write(1, "", 0);
	check("write 0 bytes", ft_ret == libc_ret);
}

/* 評価シート: stdin(skip in auto), open fd, wrong fd, return value, errno check */
static void test_read(void)
{
	int fd1;
	int fd2;
	int write_fd;
	char buf1[128];
	char buf2[128];
	ssize_t ft_ret;
	ssize_t libc_ret;

	printf("\n=== ft_read ===\n");

	/* open file descriptor */
	write_fd = open("/tmp/libasm_test_read.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (write_fd >= 0)
	{
		write(write_fd, "read test data", 14);
		close(write_fd);

		fd1 = open("/tmp/libasm_test_read.tmp", O_RDONLY);
		fd2 = open("/tmp/libasm_test_read.tmp", O_RDONLY);
		if (fd1 >= 0 && fd2 >= 0)
		{
			memset(buf1, 0, sizeof(buf1));
			memset(buf2, 0, sizeof(buf2));
			ft_ret = ft_read(fd1, buf1, 127);
			libc_ret = read(fd2, buf2, 127);
			check("open fd return value", ft_ret == libc_ret);
			check("open fd content", strcmp(buf1, buf2) == 0);
			close(fd1);
			close(fd2);
		}
		unlink("/tmp/libasm_test_read.tmp");
	}

	/* wrong file descriptor */
	errno = 0;
	ft_ret = ft_read(-1, buf1, 10);
	int ft_errno = errno;
	errno = 0;
	libc_ret = read(-1, buf1, 10);
	int libc_errno = errno;
	check("wrong fd returns -1", ft_ret == libc_ret);
	check("wrong fd sets errno EBADF", ft_errno == libc_errno);

	/* read 0 bytes */
	fd1 = open("/dev/null", O_RDONLY);
	fd2 = open("/dev/null", O_RDONLY);
	if (fd1 >= 0 && fd2 >= 0)
	{
		ft_ret = ft_read(fd1, buf1, 0);
		libc_ret = read(fd2, buf2, 0);
		check("read 0 bytes", ft_ret == libc_ret);
		close(fd1);
		close(fd2);
	}
}

/* 評価シート: empty string, very long string */
static void test_strdup(void)
{
	char *ft_dup;
	char *libc_dup;
	char *longstr;

	printf("\n=== ft_strdup ===\n");

	ft_dup = ft_strdup("");
	libc_dup = strdup("");
	check("empty string", strcmp(ft_dup, libc_dup) == 0);
	free(ft_dup);
	free(libc_dup);

	ft_dup = ft_strdup("hello");
	libc_dup = strdup("hello");
	check("hello", strcmp(ft_dup, libc_dup) == 0);
	free(ft_dup);
	free(libc_dup);

	ft_dup = ft_strdup("42 Tokyo libasm");
	libc_dup = strdup("42 Tokyo libasm");
	check("normal string", strcmp(ft_dup, libc_dup) == 0);
	free(ft_dup);
	free(libc_dup);

	longstr = make_long_string(10000);
	if (longstr)
	{
		ft_dup = ft_strdup(longstr);
		libc_dup = strdup(longstr);
		check("very long string (10000)", strcmp(ft_dup, libc_dup) == 0);
		check("strdup is independent copy", ft_dup != longstr);
		free(ft_dup);
		free(libc_dup);
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

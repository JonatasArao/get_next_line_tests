/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_strdup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:31:05 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 10:13:06 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

static int capture_segfault_ft_strdup(char *(*f)(const char *), const char *src)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// Child process executes the test
		f(src);
		exit(0);
	}
	else if (pid > 0)
	{
		// Parent process waits for the child
		int status;
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV)
		{
			return 1;
		}
		return 0;
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

MU_TEST(test_ft_strdup_is_null_terminated)
{
	// ARRANGE
	char	expected_result[3];
	char	*actual_result;
	char	message[53];

	// ACT
	strcpy(expected_result, "42");
	actual_result = ft_strdup("42");
	strcpy(message, "Expected the duplicated string to be null-terminated");

	// ASSERT
	mu_assert(actual_result[2] == '\0', message);

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_forty_two)
{
	// ARRANGE
	char	expected_result[3];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "42");
	actual_result = ft_strdup("42");

	// ASSERT
	if (actual_result[3] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 3 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_empty_string)
{
	// ARRANGE
	char	expected_result[1];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "");
	actual_result = ft_strdup("");

	// ASSERT
	if (actual_result[1] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 1 byte");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_null_char_in_middle)
{
	// ARRANGE
	char	expected_result[7];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "String");
	actual_result = ft_strdup("String\0with null");

	// ASSERT
	if (actual_result[7] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 7 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_long_string)
{
	// ARRANGE
	char	expected_result[59];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "This is a very long string to test the ft_strdup function.");
	actual_result = ft_strdup("This is a very long string to test the ft_strdup function.");

	// ASSERT
	if (actual_result[59] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 59 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_special_characters)
{
	// ARRANGE
	char	expected_result[9];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "Hello\n\t!");
	actual_result = ft_strdup("Hello\n\t!");

	// ASSERT
	if (actual_result[9] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 9 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_spaces)
{
	// ARRANGE
	char	expected_result[4];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "   ");
	actual_result = ft_strdup("   ");

	// ASSERT
	if (actual_result[4] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 4 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_unicode_string)
{
	// ARRANGE
	char	expected_result[16];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "こんにちは");
	actual_result = ft_strdup("こんにちは");

	// ASSERT
	if (actual_result[16] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 16 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_escape_characters)
{
	// ARRANGE
	char	expected_result[14];
	char	*actual_result;

	// ACT
	strcpy(expected_result, "Hello\nWorld\t!");
	actual_result = ft_strdup("Hello\nWorld\t!");

	// ASSERT
	if (actual_result[14] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 14 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_very_long_string)
{
	// ARRANGE
	char	expected_result[10000];
	char	*actual_result;

	// ACT
	memset(expected_result, 'a', 9999);
	expected_result[9999] = '\0';
	actual_result = ft_strdup(expected_result);

	// ASSERT
	if (actual_result[10000] == (char) 0xFF)
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 10000 bytes");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strdup_null_pointer)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_strdup(&ft_strdup, NULL);
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST(test_ft_strdup_malloc_fail)
{
	// ARRANGE
	void	*expected_result;
	void	*actual_result;
	int		segfaulted;

	// ACT
	expected_result = NULL;
	mock_malloc_failure_active = 1;
	segfaulted = capture_segfault_ft_strdup(&ft_strdup, "42");
	if (!segfaulted)
		actual_result = ft_strdup("42");
	mock_malloc_failure_active = 0;

	// ASSERT
	if (!segfaulted)
	{
		mu_assert(actual_result == expected_result, "Expected NULL pointer");
		free(actual_result);
	}
	else
		mu_fail("Function should not cause a segmentation fault.");
}

MU_TEST_SUITE(ft_strdup_test_suite)
{
	MU_RUN_TEST(test_ft_strdup_is_null_terminated);
	MU_RUN_TEST(test_ft_strdup_forty_two);
	MU_RUN_TEST(test_ft_strdup_empty_string);
	MU_RUN_TEST(test_ft_strdup_null_char_in_middle);
	MU_RUN_TEST(test_ft_strdup_long_string);
	MU_RUN_TEST(test_ft_strdup_special_characters);
	MU_RUN_TEST(test_ft_strdup_spaces);
	MU_RUN_TEST(test_ft_strdup_unicode_string);
	MU_RUN_TEST(test_ft_strdup_escape_characters);
	MU_RUN_TEST(test_ft_strdup_very_long_string);
	MU_RUN_TEST(test_ft_strdup_null_pointer);
	MU_RUN_TEST(test_ft_strdup_malloc_fail);
}

int	test_ft_strdup(void) {
	mock_malloc_memset_active = 1;
	MU_RUN_SUITE(ft_strdup_test_suite);
	MU_REPORT();
	mock_malloc_memset_active = 0;
	return MU_EXIT_CODE;
}

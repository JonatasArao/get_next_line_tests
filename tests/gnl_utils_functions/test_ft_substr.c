/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_substr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:42:11 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 08:33:09 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

static int	capture_segfault_ft_substr(char *(*f)(const char *, unsigned int, size_t), char const *s, unsigned int start, size_t len)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// Child process executes the test
		f(s, start, len);
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

MU_TEST(test_ft_substr_is_null_terminated)
{
	// ARRANGE
	char	*actual_result;

	// ACT
	actual_result = ft_substr("42", 0, 2);
	if (!actual_result) {
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert(actual_result[2] == '\0', "Expected the duplicated string to be null-terminated");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_0_len_0)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 0, 0);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_0_len_16)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("forty two school");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 0, 16);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_0_len_20)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("forty two school");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 0, 20);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	if (memcmp(&actual_result[17], "\xFF\xFF", 2) == 0)
		mu_fail("Allocation size is larger than expected for the substring");
	else if (memcmp(&actual_result[17], "\xFF", 1) != 0)
		mu_fail("Allocation size is shorter than expected for the substring");
	else
		mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_6_len_3)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("two");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 6, 3);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_10_len_6)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("school");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 10, 6);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_forty_two_school_start_20_len_5)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_substr("forty two school", 20, 5);
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_substr_null_pointer)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_substr(&ft_substr, NULL, 0, 1);
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST(test_ft_substr_malloc_fail)
{
	// ARRANGE
	void	*expected_result;
	void	*actual_result;
	int		segfaulted;

	// ACT
	expected_result = NULL;
	mock_malloc_failure_active = 1;
	segfaulted = capture_segfault_ft_substr(&ft_substr, "42", 0, 2);
	if (!segfaulted)
		actual_result = ft_substr("42", 0, 2);
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

MU_TEST_SUITE(ft_substr_test_suite)
{
	mock_malloc_memset_active = 1;
	MU_RUN_TEST(test_ft_substr_is_null_terminated);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_0_len_0);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_0_len_16);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_0_len_20);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_6_len_3);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_10_len_6);
	MU_RUN_TEST(test_ft_substr_forty_two_school_start_20_len_5);
	MU_RUN_TEST(test_ft_substr_null_pointer);
	MU_RUN_TEST(test_ft_substr_malloc_fail);
	mock_malloc_memset_active = 0;
}

int	test_ft_substr(void) {
	MU_RUN_SUITE(ft_substr_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

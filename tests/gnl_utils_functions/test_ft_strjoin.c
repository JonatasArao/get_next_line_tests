/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_strjoin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 07:21:46 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 07:22:50 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

static int capture_segfault_ft_strjoin(char *(*f)(const char *, const char *), char const *s1, char const *s2)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// Child process executes the test
		f(s1, s2);
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

MU_TEST(test_ft_strjoin_is_null_terminated)
{
	// ARRANGE
	char	*actual_result;

	// ACT
	actual_result = ft_strjoin("4", "2");
	if (!actual_result) {
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	mu_assert(actual_result[2] == '\0', "Expected the duplicated string to be null-terminated");

	// CLEANUP
	free(actual_result);
}

MU_TEST(test_ft_strjoin_s1_forty_s2_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("forty two");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_strjoin("forty ", "two");
	if (!actual_result) {
		free(expected_result);
		mu_fail("Memory allocation failed");
		return;
	}

	// ASSERT
	if (((char*) expected_result)[10] == ((char*) actual_result)[10])
		mu_assert_string_eq(expected_result, actual_result);
	else
		mu_fail("Incorrect allocation size. Expected was 10 bytes");

	// CLEANUP
	free(expected_result);
	free(actual_result);
}

MU_TEST(test_ft_strjoin_s1_empty)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("42");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_strjoin("", "42");
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

MU_TEST(test_ft_strjoin_s2_empty)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;

	// ACT
	expected_result = strdup("42");
	if (!expected_result) {
		mu_fail("Memory allocation failed");
		return;
	}
	actual_result = ft_strjoin("42", "");
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

MU_TEST(test_ft_strjoin_s1_empty_s2_empty)
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
	actual_result = ft_strjoin("", "");
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

MU_TEST(test_ft_strjoin_s1_null)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_strjoin(&ft_strjoin, NULL, "42");
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST(test_ft_strjoin_s2_null)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_strjoin(&ft_strjoin, "42", NULL);
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST(test_ft_strjoin_s1_null_s2_null)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_strjoin(&ft_strjoin, NULL, NULL);
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST(test_ft_strjoin_malloc_fail)
{
	// ARRANGE
	void	*expected_result;
	void	*actual_result;
	int		segfaulted;

	// ACT
	expected_result = NULL;
	mock_malloc_failure_active = 1;
	segfaulted = capture_segfault_ft_strjoin(&ft_strjoin, "forty ", "two");
	if (!segfaulted)
		actual_result = ft_strjoin("forty ", "two");
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

MU_TEST_SUITE(ft_strjoin_test_suite)
{
	mock_malloc_memset_active = 1;
	MU_RUN_TEST(test_ft_strjoin_is_null_terminated);
	MU_RUN_TEST(test_ft_strjoin_s1_forty_s2_two);
	MU_RUN_TEST(test_ft_strjoin_s1_empty);
	MU_RUN_TEST(test_ft_strjoin_s2_empty);
	MU_RUN_TEST(test_ft_strjoin_s1_empty_s2_empty);
	MU_RUN_TEST(test_ft_strjoin_s1_null);
	MU_RUN_TEST(test_ft_strjoin_s2_null);
	MU_RUN_TEST(test_ft_strjoin_s1_null_s2_null);
	MU_RUN_TEST(test_ft_strjoin_malloc_fail);
	mock_malloc_memset_active = 0;
}

int	test_ft_strjoin(void) {
	MU_RUN_SUITE(ft_strjoin_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_strchr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 09:38:23 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 07:40:42 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

static int	capture_segfault_ft_strchr(char *(*f)(const char *, int), const char *s, int c)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// Child process executes the test
		f(s, c);
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

MU_TEST(test_ft_strchr_f_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = string;
	actual_result = ft_strchr(string, 'f');
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_t_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = &string[3];
	actual_result = ft_strchr(string, 't');
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_o_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = &string[1];
	actual_result = ft_strchr(string, 'o');
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_null_char_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = &string[9];
	actual_result = ft_strchr(string, '\0');
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_char_y_above_255_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = &string[4];
	actual_result = ft_strchr(string, 377);
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_four_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;

	// ACT
	string = strdup("forty two");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = NULL;
	actual_result = ft_strchr(string, '4');

	// ASSERT
	mu_assert(actual_result == expected_result, "Expected NULL pointer");

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_non_ascii_in_forty_two)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("forty two");
	string[1] = (char) 155;
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = &string[1];
	actual_result = ft_strchr(string, 155);
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_empty_string)
{
	// ARRANGE
	char	*expected_result;
	char	*actual_result;
	char	*string;
	char	message[50];

	// ACT
	string = strdup("");
	if (!string) {
		mu_fail("Memory allocation failed");
		return;
	}
	expected_result = NULL;
	actual_result = ft_strchr(string, ' ');
	snprintf(message, sizeof(message), "Expected memory address %p but was %p\n", (void *)expected_result, (void *)actual_result);

	// ASSERT
	mu_assert(expected_result == actual_result, message);

	// CLEANUP
	free(string);
}

MU_TEST(test_ft_strchr_null_pointer)
{
	int		expected_result;
	int		actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault_ft_strchr(&ft_strchr, NULL, '\0');
	mu_assert(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST_SUITE(ft_strchr_test_suite)
{
	MU_RUN_TEST(test_ft_strchr_f_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_t_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_o_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_null_char_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_char_y_above_255_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_four_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_non_ascii_in_forty_two);
	MU_RUN_TEST(test_ft_strchr_empty_string);
	MU_RUN_TEST(test_ft_strchr_null_pointer);
}

int	test_ft_strchr(void) {
	MU_RUN_SUITE(ft_strchr_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

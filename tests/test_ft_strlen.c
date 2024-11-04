/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_strlen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 11:34:16 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/04 15:32:18 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

MU_TEST(test_ft_strlen_null_string)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 0;
	actual_result = ft_strlen("");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_fortytwo)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 8;
	actual_result = ft_strlen("fortytwo");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_forty_two)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 9;
	actual_result = ft_strlen("forty two");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_42)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 2;
	actual_result = ft_strlen("42");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_Forty_Two_42)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 12;
	actual_result = ft_strlen("forty two 42");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_string_with_special_chars)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 11;
	actual_result = ft_strlen("hello\nworld");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_string_with_null_char)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 5;
	actual_result = ft_strlen("hello\0world");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_long_string)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];
	char	long_string[1001];

	// ACT
	memset(long_string, 'a', 1000);
	long_string[1000] = '\0';
	expected_result = 1000;
	actual_result = ft_strlen(long_string);
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_multibyte_chars)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 15; // "こんにちは" in UTF-8 is 15 bytes
	actual_result = ft_strlen("こんにちは");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_escape_chars)
{
	// ARRANGE
	size_t	expected_result;
	size_t	actual_result;
	char	message[60];

	// ACT
	expected_result = 11;
	actual_result = ft_strlen("hello\tworld");
	snprintf(message, sizeof(message), "%zu expected but was %zu\n", expected_result, actual_result);
	// ASSERT
	mu_assert(expected_result == actual_result, message);
}

MU_TEST(test_ft_strlen_null_pointer)
{
	int	expected_result;
	int	actual_result;

	// ACT & ASSERT
	expected_result = 1;
	actual_result = capture_segfault((void (*)(void *))ft_strlen, NULL);
	mu_assert_warning(expected_result == actual_result, "Expected segmentation fault, but it did not occur.");
}

MU_TEST_SUITE(ft_strlen_test_suite)
{
	MU_RUN_TEST(test_ft_strlen_null_string);
	MU_RUN_TEST(test_ft_strlen_fortytwo);
	MU_RUN_TEST(test_ft_strlen_forty_two);
	MU_RUN_TEST(test_ft_strlen_42);
	MU_RUN_TEST(test_ft_strlen_Forty_Two_42);
	MU_RUN_TEST(test_ft_strlen_string_with_special_chars);
	MU_RUN_TEST(test_ft_strlen_string_with_null_char);
	MU_RUN_TEST(test_ft_strlen_long_string);
	MU_RUN_TEST(test_ft_strlen_multibyte_chars);
	MU_RUN_TEST(test_ft_strlen_escape_chars);
	MU_RUN_TEST(test_ft_strlen_null_pointer);
}

int	test_ft_strlen(void)
{
	MU_RUN_SUITE(ft_strlen_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

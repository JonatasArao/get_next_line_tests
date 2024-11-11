/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_next_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 09:07:39 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/11 14:25:33 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "minunit.h"

static void	create_test_files(void)
{
	FILE *file;

	file = fopen("test_file.txt", "w");
	if (file)
	{
		fprintf(file, "Line 1\nLine 2\nLine 3");
		fclose(file);
	}

	file = fopen("empty_file.txt", "w");
	if (file)
	{
		fclose(file);
	}

	file = fopen("single_line.txt", "w");
	if (file)
	{
		fprintf(file, "Single line content\n");
		fclose(file);
	}

	file = fopen("no_newline.txt", "w");
	if (file)
	{
		fprintf(file, "No newline at end");
		fclose(file);
	}
}

static void	delete_test_files(void)
{
	remove("test_file.txt");
	remove("empty_file.txt");
	remove("single_line.txt");
	remove("no_newline.txt");
}

MU_TEST(test_get_next_line_multiple_lines)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT & ASSERT
	fd = open("test_file.txt", O_RDONLY);
	expected_result = "Line 1\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	expected_result = "Line 2\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	free(actual_result);
	expected_result = "Line 3";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	expected_result = NULL;
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST(test_get_next_line_empty_file)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT & ASSERT
	fd = open("empty_file.txt", O_RDONLY);
	expected_result = NULL;
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST(test_get_next_line_single_line)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT & ASSERT
	fd = open("single_line.txt", O_RDONLY);
	expected_result = "Single line content\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	expected_result = NULL;
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST(test_get_next_line_no_newline)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT & ASSERT
	fd = open("no_newline.txt", O_RDONLY);
	expected_result = "No newline at end";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	expected_result = NULL;
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST_SUITE(get_next_line_test_suite)
{
	MU_RUN_TEST(test_get_next_line_empty_file);
	MU_RUN_TEST(test_get_next_line_single_line);
	MU_RUN_TEST(test_get_next_line_multiple_lines);
	MU_RUN_TEST(test_get_next_line_no_newline);
}

int main(void)
{
	create_test_files();
	MU_RUN_SUITE(get_next_line_test_suite);
	MU_REPORT();
	delete_test_files();
	return MU_EXIT_CODE;
}

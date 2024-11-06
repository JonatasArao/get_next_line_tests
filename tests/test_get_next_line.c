/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_next_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 09:07:39 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 09:19:40 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

void create_test_file(void)
{
	int fd;
	const char *content;

	content = "Hello, World!\nLine 1\nLine 2\nLine 3\nNo newline at end";
	fd = open("test_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd, content, strlen(content));
	close(fd);
}

void delete_test_file(void)
{
	remove("test_file.txt");
}

MU_TEST(test_get_next_line)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT
	fd = open("test_file.txt", O_RDONLY);
	expected_result = "Hello, World!\n";
	actual_result = get_next_line(fd);

	// ASSERT
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST(test_get_next_line_multiple_lines)
{
	// ARRANGE
	int		fd;
	char	*expected_result;
	char	*actual_result;

	// ACT & ASSERT
	fd = open("test_file.txt", O_RDONLY);
	get_next_line(fd); // Skip first line
	expected_result = "Line 1\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	free(actual_result);
	expected_result = "Line 2\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	free(actual_result);
	expected_result = "Line 3\n";
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

	// ACT
	fd = open("test_file.txt", O_RDONLY);
	get_next_line(fd); // Skip first line
	get_next_line(fd); // Skip second line
	get_next_line(fd); // Skip third line
	get_next_line(fd); // Skip fourth line
	expected_result = "No newline at end\n";
	actual_result = get_next_line(fd);

	// ASSERT
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

	// ACT
	fd = open("test_file.txt", O_RDONLY);
	get_next_line(fd); // Skip first line
	get_next_line(fd); // Skip second line
	get_next_line(fd); // Skip third line
	get_next_line(fd); // Skip fourth line
	expected_result = NULL;
	actual_result = get_next_line(fd);

	// ASSERT
	mu_assert(expected_result == actual_result, "Expected NULL for empty file");

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST_SUITE(get_next_line_test_suite)
{
	create_test_file();
	MU_RUN_TEST(test_get_next_line);
	MU_RUN_TEST(test_get_next_line_multiple_lines);
	MU_RUN_TEST(test_get_next_line_no_newline);
	MU_RUN_TEST(test_get_next_line_empty_file);
	delete_test_file();
}

int	main(void) {
	MU_RUN_SUITE(get_next_line_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_next_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 09:07:39 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 13:52:26 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
# include "minunit.h"

void create_test_file(void)
{
	int fd;
	const char *content;

	content = "Line 1\nLine 2\nLine 3";
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

	// ACT & ASSERT
	fd = open("test_file.txt", O_RDONLY);
	expected_result = "Line 1\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	free(actual_result);
	expected_result = "Line 2\n";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);
	free(actual_result);
	expected_result = "Line 3";
	actual_result = get_next_line(fd);
	mu_assert_string_eq(expected_result, actual_result);

	// CLEANUP
	free(actual_result);
	close(fd);
}

MU_TEST_SUITE(get_next_line_test_suite)
{
	create_test_file();
	MU_RUN_TEST(test_get_next_line);
	delete_test_file();
}

int	main(void) {
	MU_RUN_SUITE(get_next_line_test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

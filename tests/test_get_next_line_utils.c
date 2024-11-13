/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_get_next_line_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 09:07:39 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/13 18:34:05 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	main(void)
{
	printf(PRINTCYAN("\nTesting: ft_strchr\n"));
	test_ft_strchr();
	printf(PRINTCYAN("\nTesting: ft_substr\n"));
	test_ft_substr();
	printf(PRINTCYAN("\nTesting: ft_strjoin\n"));
	test_ft_strjoin();
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 08:46:21 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/06 10:12:50 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_TESTS_H
# define GNL_TESTS_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <string.h>
# define PRINTYELLOW(X) "\033[33m"X"\033[0m"
# define PRINTCYAN(X) "\033[1;36m"X"\033[0m"
# define mu_assert_warning(test, message) \
	MU__SAFE_BLOCK( \
		minunit_assert++; \
		if (!(test)) { \
			snprintf(minunit_last_message, MINUNIT_MESSAGE_LEN, \
					 PRINTYELLOW("%s warning:\n\t%s:%d: %s"), \
					 __func__, __FILE__, __LINE__, message); \
			minunit_status = 1; \
		} else { \
			printf(BOLDGREEN(".")); \
		} \
	)

extern int	mock_free_counter_active;
extern int	mock_free_counter;
extern int	mock_malloc_memset_active;
extern int	mock_malloc_failure_active;
extern int	mock_malloc_failure_threshold;
extern int	mock_malloc_failure_counter;
extern int	mock_malloc_counter_active;
extern int	mock_malloc_counter;

size_t	ft_strlen(const char *s);
int		test_ft_strlen(void);

char	*ft_strchr(const char *s, int c);
int		test_ft_strchr(void);

char	*ft_strdup(const char *src);
int		test_ft_strdup(void);

char	*ft_substr(char const *s, unsigned int start, size_t len);
int		test_ft_substr(void);

char	*ft_strjoin(char const *s1, char const *s2);
int		test_ft_strjoin(void);

char *get_next_line(int fd);

#endif

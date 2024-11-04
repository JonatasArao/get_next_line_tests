/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 08:47:30 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/04 10:54:36 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	capture_segfault(void (*func)(), ...)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		// Processo filho executa o teste
		va_list args;
		va_start(args, func);
		func(va_arg(args, void *));
		va_end(args);
		exit(0);
	}
	else if (pid > 0)
	{
		// Processo pai espera pelo filho
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

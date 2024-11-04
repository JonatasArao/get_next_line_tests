/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarao-de <jarao-de@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 10:17:33 by jarao-de          #+#    #+#             */
/*   Updated: 2024/11/04 09:56:42 by jarao-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

// Global variables to control the behavior of the mock free function
int mock_free_counter_active = 0; // Flag to activate counter behavior in free
int mock_free_counter; // Counter to keep track of free calls

// Mock free function
void free(void *ptr) {
	void (*original_free)(void *); // Pointer to the original free function
	char *error;

	// Get the pointer to the original free function
	original_free = dlsym(RTLD_NEXT, "free");
	if ((error = dlerror()) != NULL)
	{ // Check for errors in dlsym
		fprintf(stderr, "%s\n", error); // Print the error message
		exit(EXIT_FAILURE); // Exit the program in case of error
	}

	// If counter behavior is active, increment the counter
	if (mock_free_counter_active)
		mock_free_counter++;

	// Call the original free function
	original_free(ptr);
}

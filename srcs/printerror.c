/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printerror.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 15:19:44 by rokerjea          #+#    #+#             */
/*   Updated: 2022/03/07 15:19:46 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	printerror(char **argv, char *prob)
{
	write(STDERR_FILENO, argv[0] + 2, ft_strlen(argv[0]) - 2);
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	if (prob)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, prob, ft_strlen(prob));
	}
	write(STDERR_FILENO, "\n", 1);
}

void	printcfn(char **argv, char *prob)
{
	write(2, argv[0], ft_strlen(argv[0]));
	write(2, ": command not found: ", 21);
	write(2, prob, ft_strlen(prob));
	write(2, "\n", 1);
}

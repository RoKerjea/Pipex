/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:32:59 by rokerjea          #+#    #+#             */
/*   Updated: 2022/02/24 12:34:28 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	multi_pipex(int *filefd, int argc, char **argv, char **envp)
{
	int		pipefd[2];
	int		i;
	pid_t	pid;
	int		fd_in;

	fd_in = filefd[0];
	i = 2;
	while (i < (argc - 2))
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			dup2(fd_in, STDIN_FILENO);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			externalcommand(argv[i], envp);
		}
		close(pipefd[1]);
		fd_in = pipefd[0];
		i++;
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(filefd[1], STDOUT_FILENO);
	externalcommand(argv[i], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int	filefd[2];

	if (argc != 5)
	{
		write(1, "Program need 4 arguments\n", 25);
		return (EXIT_FAILURE);
	}
	filefd[0] = open(argv[1], O_RDONLY);
	filefd[1] = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (access(argv[1], F_OK))
	{
		write(STDERR_FILENO, "pipex: ", 7);
		write(STDERR_FILENO, argv[1], ft_strlen(argv[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		filefd[0] = 0;
	}
	if (filefd[0] < 0 || filefd[1] < 0)
		return (EXIT_FAILURE);
	multi_pipex(filefd, argc, argv, envp);
	return (0);
}

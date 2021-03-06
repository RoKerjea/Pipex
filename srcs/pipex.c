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

void	waitsegft(pid_t pid, char *argv)
{
	int	status;

	status = 0;
	waitpid(-1, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGSEGV)
		{
			ft_printnbr(pid);
			write(STDERR_FILENO, " segmentation fault ", 20);
			write(STDERR_FILENO, argv, ft_strlen(argv));
			write(STDERR_FILENO, "\n", 1);
		}
	}
	exit(status);
}

void	multi_pipex(int *filefd, int argc, char **argv, char **envp)
{
	int		pipefd[2];
	int		i;
	pid_t	pid;
	int		fd_in;

	i = 1;
	fd_in = filefd[0];
	while (++i < (argc - 2))
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			if (fd_in == 0)
				exit(EXIT_FAILURE);
			dup2(fd_in, STDIN_FILENO);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			externalcommand(argv, argv[i], envp);
		}
		close(pipefd[1]);
		fd_in = pipefd[0];
	}
	dup2(fd_in, STDIN_FILENO);
	externalcommand(argv, argv[i], envp);
}

int	inputtest(char **argv, char *input)
{
	if (access(input, F_OK))
	{
		printerror(argv, input);
		return (EXIT_FAILURE);
	}
	if (access(input, R_OK))
	{
		printerror(argv, input);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	elderchild(int *filefd, int argc, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		multi_pipex(filefd, argc, argv, envp);
	waitsegft(pid, argv[argc - 2]);
}

int	main(int argc, char **argv, char **envp)
{
	int	filefd[2];

	if (argc != 5)
	{
		write(1, "Program need 4 arguments\n", 25);
		return (EXIT_FAILURE);
	}
	if (inputtest(argv, argv[1]) == EXIT_SUCCESS)
		filefd[0] = open(argv[1], O_RDONLY);
	else
		filefd[0] = 0;
	filefd[1] = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (filefd[1] < 0)
	{
		printerror(argv, argv[argc - 1]);
		return (EXIT_FAILURE);
	}
	dup2(filefd[1], STDOUT_FILENO);
	elderchild(filefd, argc, argv, envp);
	return (0);
}

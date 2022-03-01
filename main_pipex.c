/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:32:59 by rokerjea          #+#    #+#             */
/*   Updated: 2022/02/24 12:34:28 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	child_start(int f1, char *cmd1, char **envp, int *pipe0)
{
	//add protection if dup2() < 0
	//dup2 close stdin, f1 become new stdin
	dup2(f1, STDIN_FILENO); //we want f1 to be execve() input
	dup2(pipe0[1], STDOUT_FILENO);//we want pipe0[1] to be execve() stdout
	close(pipe0[0]); /* always close the end of the pipe you don't use,
                    as long as the pipe is open, the other end will 
                    be waiting for some kind of input and will not
                    be able to finish its process*/
	close(f1);
	perror("start ");
	externalcommand(cmd1, envp);//execve function for each possible path
	exit(EXIT_FAILURE);
}

int	child_middle(char *cmd, char **envp, int pipe[2][2], int x, pid_t prevpid)
{
	int	status;

	waitpid(prevpid, &status, 0);//need wait previous
	dup2(pipe[x % 2][0], STDIN_FILENO);
	dup2(pipe[(x + 1) % 2][1], STDOUT_FILENO);
	close(pipe[x % 2][1]);
	close(pipe[(x + 1) % 2][0]);
	perror("midd1 ");
	externalcommand(cmd, envp);
	exit(EXIT_FAILURE);
}

int	child_end(int f2, char *cmd, char **envp, int *pipe, pid_t prevpid)
{
	int	status;

	waitpid(prevpid, &status, 0);// need to wait for previous child
	dup2(pipe[0], STDIN_FILENO);//end[0] is the stdin
	dup2(f2, STDOUT_FILENO);//f2 is the stdout
	close(pipe[1]);
	close(f2);
	perror("end ");
	externalcommand(cmd, envp);// execve function for each possible path
	exit(EXIT_FAILURE);
}

pid_t	*child_count(int argc)
{
	pid_t	*child_ls;

	child_ls = malloc(sizeof(pid_t) * (argc - 3));
	if (!child_ls)
		return (NULL);
	return (child_ls);
}

void	pipex(int *filefd, int argc, char **argv, char **envp)
{
	int		pipels[2][2];
	int		status;
	int		x;
	pid_t	*childls;

	childls = child_count(argc);

	pipe(pipels[0]);
	pipe(pipels[1]);
	perror("test2: ");
	childls[0] = fork();
	if (childls[0] < 0)
		return (perror("Fork1: "));
	if (childls[0] == 0) // if fork() returns 0, we are in the child process
		child_start(filefd[0], argv[2], envp, pipels[0]);
	perror("test3: ");
	x = 2;
	while (x < (argc - 3))
	{
		childls[x - 1] = fork();
		if (childls[x - 1] < 0)
			return (perror("Forkmidd: "));
		if (childls[x - 1] == 0)
			child_middle(argv[x + 1], envp, pipels, x, childls[x - 2]);
		x++;
	}
	perror("test4: ");
	childls[x - 1] = fork();
	if (childls[x - 1] < 0)
		return (perror("Forkend: "));
	if (childls[x - 1] == 0)
		child_end(filefd[1], argv[argc - 2], envp, pipels[x % 2], childls[x - 2]);
	perror("test5: ");
	close(pipels[0][0]);// this is the parent
	close(pipels[0][1]);// doing nothing
	close(pipels[1][0]);
	close(pipels[1][1]);
	waitpid(childls[0], &status, 0); // supervising the children
	perror("test6: ");
	waitpid(childls[1], &status, 0); // while they finish their tasks
	perror("test7: ");
	waitpid(childls[2], &status, 0);
	perror("test8: ");
	/*while (childls[x++])
	{
		waitpid(child[x], &status, 0);	//the status area contains termination information about the process that exited
										//probably need some security control
	}*/
}

int	main(int argc, char **argv, char **envp)
{
	int	filefd[2];

	filefd[0] = open(argv[1], O_RDONLY);
	filefd[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (filefd[0] < 0 || filefd[1] < 0)
		return (EXIT_FAILURE);
	if (argc < 5)
	{
		printf("Program need 4 arguments\n");
		return (EXIT_FAILURE);
	}
	perror("test: ");
	pipex(filefd, argc, argv, envp);
	/*printf("le premier argument est \"%s\"\n", argv[0]);
	printf("le second argument est \"%s\"\n", argv[1]);
	printf("envp 13 = \"%s\"\n", envp[13]);*/
	return (0);
}

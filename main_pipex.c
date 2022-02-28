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
#include <stdio.h>

int	child_one (int f1, char *cmd1, char **envp)
{
	//add protection if dup2() < 0
	//dup2 close stdin, f1 become new stdin
	dup2(f1, STDIN_FILNO); //we want f1 to be execve() input
	dup2(end[1], STDOUT_FILENO);//we want end[1] to be execve() stdout
	close(end[0]);  /* always close the end of the pipe you don't use,
                    as long as the pipe is open, the other end will 
                    be waiting for some kind of input and will not
                    be able to finish its process*/
	close(f1);
	externalcommand(cmd1, envp);//execve function for each possible path
	exit(EXIT_FAILURE);
}

int	child_two(int f2, char *cmd2, char **envp);
{
	int	status;

	waitpid(-1, &status, 0);
	dup2(f2, ...);//f2 is the stdout
	dup2(end[0], ...);//end[0] is the stdin
	close(end[1]);
	close(f2);
	externalcommand(cmd2, envp);// execve function for each possible path
	exit(EXIT_FAILURE);
}

int	findpathline(char **envp)
{
	int		i;
	int		j;
	char	*pathstr;

	pathstr = "PATH=";
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] == pathstr[j])
		{
			if (j == 4)
				return (i);
			j++;
		}
		i++;
	}
	printf("env has been tampered, can't find \"PATH=...\" line\n");
	return (EXIT_FAILURE);
}


char	**path_parsing(char **envp)
{
	char	*path_from_envp;
	char	*path_transformed;
	char	**mypaths;
	int		pline;

	pline = findpathline(envp);
	path_from_envp = ft_substr(envp[pline], 5, ft_strlen(envp[pline]) - 5);
	printf ("pathline without \"PATH=\" is \"%s\"\n", path_from_envp);
	path_transformed = ft_strchr_replace(path_from_envp, ':', "/:");
	mypaths = ft_split(path_transformed, ':');
	return (mypaths);
}

int	externalcommand(char *cmd, char **envp)
{
	int		i;
	char	*cmdpath;
	char	**mypaths;
	char	**cmdargs;

	mypaths = path_parsing(envp);
	cmdargs = ft_split(cmd, ' ');
	i = -1;
	while (mypaths[++i])
	{
		cmdpath = ft_strjoin(mypaths[i], cmdargs[0]); // to be protected
		//access(cmdpath, X_OK | F_OK); // to check if command is accessible and executable
		//perror("Error");
		execve(cmdpath, cmdargs, envp); //if execve succeeds, it exits
		//perror("Error");
		free(cmdpath); //if execve fails, we free and start again;
	}
	perror("Error");
	return (EXIT_FAILURE);
}

void	pipex(int f1, int f2, char **argv, char **envp)
{
	int		end[2];
	int		status;
	pid_t	child1;
	pid_t	child2;

	pipe(end);
	child1 = fork();
	if (child1 < 0)
		return (perror("Fork: "));
	if (child1 == 0) // if fork() returns 0, we are in the child process
		child_one(f1, argv[2]);
	child2 = fork();
	if (child2 < 0)
		return (perror("Fork: "));
	if (child2 == 0)
		child_two(f2, argv[3])
	close(end[0]);	// this is the parent
	close(end[1]);	// doing nothing
	waitpid(child1, &status, 0); // supervising the children
	waitpid(child2, &status, 0); // while they finish their tasks
}

int	main(int argc, char **argv, char **envp)
{
	int	f1;
	int f2;

	f1 = open(argv[1], O_RDONLY);
	f2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (f1 < 0 || f2 < 0)
		return (EXIT_FAILURE);
	if (argc != 4)
	{
		printf("Program need 4 arguments\n");
		return (EXIT_FAILURE);
	}
	pipex(f1, f2, argv, envp);
	/*printf("le premier argument est \"%s\"\n", argv[0]);
	printf("le second argument est \"%s\"\n", argv[1]);
	printf("envp 13 = \"%s\"\n", envp[13]);*/
	return (0);
}

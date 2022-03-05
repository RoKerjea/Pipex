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

void	multi_pipex(int *filefd, int argc, char **argv, char **envp)
{
	int		pipefd[2];
	int		i;
	pid_t	pid;
	int		fd_in = filefd[0];
	pid_t	prevpid;

	prevpid = 0;
	i = 2;
	while (i < (argc - 1))
	{
		pipe(pipefd);//opening pipe
		pid = fork();
		if (pid < 0)//fork failure, stop
			return(perror("Fork"));
		else if (pid == 0)//we are in the child process
		{
			dup2(fd_in, STDIN_FILENO);
			if (i < (argc - 2))//si on est pas dans le dernier process child on utilise l'ecriture du pipe comme STDOUT
				dup2(pipefd[1], STDOUT_FILENO);
			else// on dans le dernier process child
				dup2(filefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			if (prevpid > 0)
				waitpid(prevpid, NULL, 0);//need to wait for previous pid to act enough
			externalcommand(argv[i], envp);//on execve le process actuel avec les fd modifies et la commande 
			exit(127);//execve failed
		}
		else//we are still in parent process while
		{/*
			prevpid = pid;
			char *messg = "we are in parent process, waiting for pid : \"";
			char *messgend = "\"\n";
			ft_printf("%s%i%s" ,messg, pid, messgend);*/
			close(pipefd[1]);//on ferme l'extremite lecture du pipe,que le parent n'utilise pas
			fd_in = pipefd[0];// tete lecture du pipe pour next child input
			i++;
		}
	}
	/*
	char *messg = "we are in parent process, waiting for last process pid : \"";
	char *messgend = "\"\n";
	ft_printf("%s%i%s" ,messg, pid, messgend);
	waitpid(pid, NULL, 0);//on attend le process child
	char *messgf = "FINI !";
	ft_printf("%s%s" ,messgf, messgend);*/
}

int	main(int argc, char **argv, char **envp)
{
	int	filefd[2];

	if (argc < 5)	
	{
		ft_printf("Program need 4 arguments\n");
		return (EXIT_FAILURE);
	}
	if (access(argv[1], F_OK))
	{
			write(STDERR_FILENO, "pipex: ", 7);
			write(STDERR_FILENO, argv[1], ft_strlen(argv[1]));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
			return (0);
	}
	filefd[0] = open(argv[1], O_RDONLY);
	filefd[1] = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (filefd[0] < 0 || filefd[1] < 0)
		return (EXIT_FAILURE);
	
	multi_pipex(filefd, argc, argv, envp);
	return (0);
}

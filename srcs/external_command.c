/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 15:38:19 by rokerjea          #+#    #+#             */
/*   Updated: 2022/03/01 15:38:23 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
	perror("env has been tampered, can't find \"PATH=...\" line\n");
	return (EXIT_FAILURE);
}

char	*path_parsing(char *cmd, char **envp)
{
	char	*path_from_envp;
	char	*cmdpath;
	char	**mypaths;
	int		i;

	i = findpathline(envp);
	path_from_envp = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	mypaths = ft_split(ft_strchr_replace(path_from_envp, ':', "/:"), ':');
	free(path_from_envp);
	i = -1;
	while (mypaths[++i])
	{
		cmdpath = ft_strjoin(mypaths[i], cmd);
		if (access(cmdpath, X_OK | F_OK) == 0)
		{
			free(mypaths);
			return (cmdpath);
		}
		free(cmdpath);
	}
	free(mypaths);
	return (NULL);
}

int	externalcommand(char *cmd, char **envp)
{
	char	*cmdpath;
	char	**cmdargs;

	cmdargs = ft_split(cmd, ' ');
	if (cmdargs[0][0] == '/')
		execve(cmdargs[0], cmdargs, envp);
	cmdpath = path_parsing(cmdargs[0], envp);
	execve(cmdpath, cmdargs, envp);
	free(cmdpath);
	free(cmdargs);
	write(2, "pipex: ", 7);
	write(2, " command not found: \n", 20);
	write(2, cmd, ft_strlen(cmd));
	exit (127);
}

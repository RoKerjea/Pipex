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

void	freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*getpath(char *cmd, char **envp)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
		path = cmd;
	else
		path = path_parsing(cmd, envp);
	return (path);
}

char	*path_parsing(char *cmd, char **envp)
{
	char	*path_from_envp;
	char	*path_edited;
	char	*cmdpath;
	char	**mypaths;
	int		i;

	i = findpathline(envp);
	path_from_envp = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	path_edited = ft_strchr_replace(path_from_envp, ':', "/:");
	mypaths = ft_split(path_edited, ':');
	free(path_edited);
	free(path_from_envp);
	i = -1;
	while (mypaths[++i])
	{
		cmdpath = ft_strjoin(mypaths[i], cmd);
		if (access(cmdpath, X_OK | F_OK) == 0)
		{
			freetab(mypaths);
			return (cmdpath);
		}
		free(cmdpath);
	}
	freetab(mypaths);
	return (NULL);
}

int	externalcommand(char *cmd, char **envp)
{
	char	*cmdpath;
	char	**cmdargs;

	cmdargs = ft_split(cmd, ' ');
	cmdpath = getpath(cmdargs[0], envp);
	if (cmdpath != NULL)
		execve(cmdpath, cmdargs, envp);
	free(cmdpath);
	write(2, "pipex: ", 7);
	write(2, " command not found: ", 20);
	write(2, cmdargs[0], ft_strlen(cmdargs[0]));
	write(2, "\n", 1);
	freetab(cmdargs);
	exit (127);
}

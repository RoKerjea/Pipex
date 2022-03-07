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
	write(STDERR_FILENO, "env problem, can't find \"PATH=...\" line\n", 41);
	exit (EXIT_FAILURE);
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
			ft_freetab(mypaths);
			return (cmdpath);
		}
		free(cmdpath);
	}
	ft_freetab(mypaths);
	return (NULL);
}

int	testcmd(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] == ' ' || cmd[i] == '\0')
	{
		if (cmd[i] == '\0')
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	externalcommand(char *cmd, char **envp)
{
	char	*cmdpath;
	char	**cmdargs;

	if (cmd[0] == '\0')
		exit(EXIT_FAILURE);
	if (testcmd(cmd) == EXIT_SUCCESS)
	{
		cmdargs = ft_split(cmd, ' ');
		cmdpath = getpath(cmdargs[0], envp);
		if (cmdpath != NULL)
			execve(cmdpath, cmdargs, envp);
		free(cmdpath);
		ft_freetab(cmdargs);
	}
	write(2, "pipex: command not found: ", 26);
	write(2, cmd, ft_strlen(cmd));
	write(2, "\n", 1);
	exit (127);
}

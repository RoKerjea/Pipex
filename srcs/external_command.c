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

int	findpathline(char *cmd, char **envp, char **argv)
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
	printerror3(argv, cmd);
	return (EXIT_FAILURE);
}

char	*getpath(char **argv, char *cmd, char **envp)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == -1)
		{
			printerror2(argv, cmd);
		}
		if (access(cmd, X_OK) == -1)
		{
			printerror2(argv, cmd);
		}
		path = cmd;
	}
	else
		path = path_parsing(cmd, envp, argv);
	if (path == NULL)
		return (NULL);
	if (access(path, F_OK) == 0 && access(path, X_OK) == -1)
	{
		printerror2(argv, cmd);
	}
	return (path);
}

char	*path_parsing(char *cmd, char **envp, char **argv)
{
	char	*path_from_envp;
	char	*path_edited;
	char	*cmdpath;
	char	**mypaths;
	int		i;

	i = findpathline(cmd, envp, argv);
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

int	ft_strspace(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\0')
	{
		if (str[i] == '\0')
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	externalcommand(char **argv, char *cmd, char **envp)
{
	char	*cmdpath;
	char	**cmdargs;

	if (cmd[0] == '\0')
	{
		access("cmd", X_OK);
		printerror(argv, cmd);
		exit(EXIT_FAILURE);
	}
	if (ft_strspace(cmd) == EXIT_SUCCESS)
	{
		cmdargs = ft_split(cmd, ' ');
		cmdpath = getpath(argv, cmdargs[0], envp);
		if (cmdpath)
		{
			execve(cmdpath, cmdargs, envp);
			free(cmdpath);
		}
		ft_freetab(cmdargs);
	}
	if (errno == 13)
		printerror(argv, cmd);
	else
		printcfn(argv, cmd);
	exit (127);
}

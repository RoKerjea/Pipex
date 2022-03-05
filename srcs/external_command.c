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

char	**path_parsing(char **envp)
{
	char	*path_from_envp;
	char	*path_transformed;
	char	**mypaths;
	int		pline;

	pline = findpathline(envp);
	path_from_envp = ft_substr(envp[pline], 5, ft_strlen(envp[pline]) - 5);
	path_transformed = ft_strchr_replace(path_from_envp, ':', "/:");
	mypaths = ft_split(path_transformed, ':');
	if (!path_from_envp || !path_transformed || !mypaths)
	{
	free(path_from_envp);
	free(path_transformed);
	return (NULL);
	}	
	free(path_from_envp);
	free(path_transformed);
	return (mypaths);
}

int	externalcommand(char *cmd, char **envp)
{
	int		i;
	char	*cmdpath;
	char	**mypaths;
	char	**cmdargs;

	cmdargs = ft_split(cmd, ' ');
	if (cmdargs[0][0] == '/')
		execve(cmdargs[0], cmdargs, envp);
	mypaths = path_parsing(envp);
	i = -1;
	while (mypaths[++i])
	{
		cmdpath = ft_strjoin(mypaths[i], cmdargs[0]);
		if (access(cmdpath, X_OK | F_OK) == 0)
			execve(cmdpath, cmdargs, envp);
		free(cmdpath);
	}
	free(cmdargs);
	free(mypaths);
	write(2, "pipex: ", 7);
	write(2, " command not found: \n", 20);
	write(2, cmd, ft_strlen(cmd));
	exit (127);
}

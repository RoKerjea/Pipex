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

#include "pipex.h"

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
	//printf ("pathline without \"PATH=\" is \"%s\"\n", path_from_envp);
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
	char *messgend = "\"\n";
	/*char *messg1 = "start of while for cmd : \"";
	
	ft_printf("%s%s%s", messg1, cmd, messgend);*/
	while (mypaths[++i])
	{
		cmdpath = ft_strjoin(mypaths[i], cmdargs[0]); // to be protected
		if (access(cmdpath, X_OK | F_OK) == 0) // to check if command is accessible and executable
		{
			char *messg = "cmd : \"";
			char *messg2 = "\" is at path : \"";
			ft_printf("%s%s%s%s%s", messg, cmdargs[0], messg2, cmdpath, messgend);
			execve(cmdpath, cmdargs, envp);
		}
		//perror("Error");
		/*char *messg = "trying cmd : \"";
		char *messg2 = "\" at path : \"";
		ft_printf("%s%s%s%s%s", messg, cmd, messg2, cmdpath, messgend);*/
		//execve(cmdpath, cmdargs, envp); //if execve succeeds, it exits
		//perror("Error");
		free(cmdpath); //if execve fails, we free and start again;
	}
	char *messg1 = "can't find command : \"";
	ft_printf("%s%s%s", messg1, cmd, messgend);
	perror("Error");
	return (EXIT_FAILURE);
}

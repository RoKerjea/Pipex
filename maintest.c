#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
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

int	main(int argc, char **argv, char **envp)
{
	int		i;
	char	**mypaths;
	int		j;

	j = 0;
	i = findpathline(envp);
	printf("line %i of envp is %s\n", i, envp[i]);
	mypaths = path_parsing(envp);
	while (mypaths[j])
	{
		printf("path number %i = \"%s\"\n", j, mypaths[j]);
		j++;
	}
	externalcommand (argv[1], envp);
	/*access("/Users/rokerjea/Pipex/PrLine", F_OK | R_OK | X_OK);
	perror("Error");
	printf("error number is : %i", errno);	
	char **cmdargs = ft_split("PrLine", ' ');
	execve("/Users/rokerjea/Pipex/PrLine", cmdargs, envp);
	perror("Error");*/
	return (0);
}

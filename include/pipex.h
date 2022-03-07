/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 12:32:25 by rokerjea          #+#    #+#             */
/*   Updated: 2022/03/05 19:00:31 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>

unsigned long	ft_strlen(const char *s);
char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strchr_replace(char *str, char needle, char *stuffing);
int				ft_strchr(const char *s, char t);
int				externalcommand(char *cmd, char **envp);
char			*path_parsing(char *cmd, char **envp);
int				findpathline(char **envp);

#endif

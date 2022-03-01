/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_replace.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 17:53:37 by rokerjea          #+#    #+#             */
/*   Updated: 2022/02/28 17:53:39 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	needle_in_str(char *str, char needle)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == needle)
			j++;
		i++;
	}
	return (j);
}

char	*ft_strchr_replace(char *str, char needle, char *stuffing)
{
	int		j;
	int		k;
	char	*strres;

	j = 0;
	strres = malloc((ft_strlen(str) + ((ft_strlen(stuffing) - 1)
					* needle_in_str(str, needle)) + 2));
	if (!strres)
		return (NULL);
	k = 0;
	while (*str)
	{
		if (*str == needle)
		{
			while (stuffing[k])
				strres[j++] = stuffing[k++];
			k = 0;
			str++;
		}
		else
			strres[j++] = *str++;
	}
	strres[j] = '/';
	strres[++j] = '\0';
	return (strres);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rokerjea <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 19:41:48 by rokerjea          #+#    #+#             */
/*   Updated: 2022/03/24 19:41:52 by rokerjea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	ft_printchar(char c)
{
	return (write (1, &c, 1));
}

int	ft_printnbr(int nb)
{
	int		count;
	long	n;

	n = nb;
	count = 0;
	if (n < 0)
	{
		ft_printchar('-');
		n *= -1;
		count++;
	}
	if (n >= 10)
		count += ft_printnbr(n / 10);
	count += ft_printchar('0' + n % 10);
	return (count);
}

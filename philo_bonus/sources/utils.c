/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:20:21 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/22 11:17:58 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	message(t_philo_parent *philo_parent, t_philo_child philo, char *str)
{
	sem_wait(philo_parent->sem_printf);
	printf("%zu %d %s\n", get_time() - philo_parent->start_time - 1000,
		philo.nb_philo, str);
	sem_post(philo_parent->sem_printf);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_atoi(char *str, int *num)
{
	long	number;
	int		i;

	i = 0;
	if (ft_strlen(str) > 10)
	{
		*num = 0;
		return (0);
	}
	number = 0;
	while (str[i] >= '0' && str[i] <= '9')
		number = number * 10 + str[i++] - '0';
	if (number > INT_MAX)
	{
		*num = 0;
		return (0);
	}
	*num = (int) number;
	return (str[i] == '\0');
}

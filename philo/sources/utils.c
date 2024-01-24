/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:20:21 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 22:29:25 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	message(t_data *data, t_philo philo, char *str, int boolean)
{
	int	bool_printf;

	pthread_mutex_lock(&(data->mutex_printf));
	bool_printf = data->bool_printf;
	if (bool_printf == 1)
	{
		printf("%zu %d %s\n", get_time() - philo.start_time - 1000,
			philo.nb_philo, str);
		if (ft_strcmp(str, "died") == 0 || boolean == 0)
			data->bool_printf = 0;
	}
	pthread_mutex_unlock(&(data->mutex_printf));
}

void	ft_usleep(t_data *data, size_t t, int first_fork, int second_fork)
{
	size_t	curr;

	curr = get_time();
	while (get_time() - curr < t)
	{
		check_for_dead(data, first_fork, second_fork);
		usleep(100);
	}
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

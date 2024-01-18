/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 14:20:21 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/18 19:49:16 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	message(t_data *data, char *str, int nb_philo)
{
	pthread_mutex_lock(&(data->mutex_printf));
	printf("%zu %d %s\n", get_time() - data->start_time, nb_philo, str);
	pthread_mutex_unlock(&(data->mutex_printf));
}

void	ft_usleep(size_t t)
{
	size_t	curr;

	curr = get_time();
	while (get_time() - curr < t)
		usleep(100);
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
	int	number;
	int	i;

	i = 0;
	if (ft_strlen(str) > 10)
		return (0);
	number = 0;
	while (str[i] >= '0' && str[i] <= '9')
		number = number * 10 + str[i++] - '0';
	*num = number;
	return (str[i] == '\0');
}

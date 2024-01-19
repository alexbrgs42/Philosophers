/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:47:53 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/19 15:08:49 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_mutex_forks_var(t_data *data)
{
	int	i;

	i = 0;
	data->mutex_forks_var = malloc(data->number_of_philosophers
			* sizeof(pthread_mutex_t));
	if (data->mutex_forks == NULL)
		return (first_free(data, 0, 0, 0));
	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_mutex_init(&(data->mutex_forks_var[i]), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(data->mutex_forks_var[i]));
			first_free(data, 0, 0, 0);
			free(data->mutex_forks_var);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_mutex_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_mutex_init(&(data->mutex_forks[i]), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(data->mutex_forks[i]));
			free(data->forks);
			free(data->mutex_forks);
			return (0);
		}
		i++;
	}
	i = 0;
	if (pthread_mutex_init(&(data->mutex_begin), NULL) != 0)
	{
		while (i < data->number_of_philosophers)
			pthread_mutex_destroy(&(data->mutex_forks[i++]));
		free(data->forks);
		free(data->mutex_forks);
		return (0);
	}
	return (init_mutex_forks_var(data));
}

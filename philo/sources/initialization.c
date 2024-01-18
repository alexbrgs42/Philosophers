/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 21:03:54 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/18 19:51:48 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_data(t_data *data, char *argv[])
{
	if (init_numbers(data, argv) == 0)
		return (0);
	if (init_forks(data) == 0)
		return (0);
	if (pthread_mutex_init(&(data->mutex_printf), NULL) != 0)
		return (first_free(data, 0, 0, 0));
	if (pthread_mutex_init(&(data->mutex_struct), NULL) != 0)
		return (first_free(data, 1, 0, 0));
	if (pthread_mutex_init(&(data->mutex_forks_var), NULL) != 0)
		return (first_free(data, 1, 1, 0));
	if (pthread_mutex_init(&(data->mutex_died), NULL) != 0)
		return (first_free(data, 1, 1, 1));
	if (pthread_mutex_init(&(data->mutex_meals), NULL) != 0)
	{
		pthread_mutex_destroy(&(data->mutex_died));
		return (first_free(data, 1, 1, 1));
	}
	data->philo = malloc(data->number_of_philosophers * sizeof(pthread_t));
	if (data->philo == NULL)
	{
		pthread_mutex_destroy(&(data->mutex_died));
		pthread_mutex_destroy(&(data->mutex_meals));
		return (first_free(data, 1, 1, 1));
	}
	return (1);
}

int	init_numbers(t_data *data, char *argv[])
{
	int	return_val;

	return_val = 0;
	data->counter = 0;
	data->died = 0;
	data->philo_done = 0;
	return_val += ft_atoi(argv[1], &(data->number_of_philosophers));
	return_val += ft_atoi(argv[2], &(data->time_to_die));
	return_val += ft_atoi(argv[3], &(data->time_to_eat));
	return_val += ft_atoi(argv[4], &(data->time_to_sleep));
	if (argv[5] != NULL)
		return_val += ft_atoi(argv[5],
				&(data->number_of_times_each_philosopher_must_eat));
	else
	{
		return_val++;
		data->philo_done = -1;
		data->number_of_times_each_philosopher_must_eat = -1;
	}
	if (data->number_of_philosophers == 0 || data->time_to_die == 0
		|| data->time_to_eat == 0 || data->time_to_sleep == 0
		|| data->number_of_times_each_philosopher_must_eat == 0)
		return (0);
	return (return_val == 5);
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
	return (1);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(data->number_of_philosophers * sizeof(int));
	if (data->forks == NULL)
		return (0);
	while (i < data->number_of_philosophers)
		(data->forks)[i++] = 1;
	data->mutex_forks = malloc(data->number_of_philosophers
			* sizeof(pthread_mutex_t));
	if (data->mutex_forks == NULL)
	{
		free(data->forks);
		return (0);
	}
	return (init_mutex_forks(data));
}

void	init_t_philo(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&(data->mutex_struct));
	philo->tts = data->time_to_sleep;
	philo->tte = data->time_to_eat;
	philo->ttd = data->time_to_die;
	philo->total = data->number_of_philosophers;
	philo->nb_philo = data->counter++;
	if (philo->nb_philo == 0)
		data->start_time = get_time();
	pthread_mutex_unlock(&(data->mutex_struct));
}

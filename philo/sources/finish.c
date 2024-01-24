/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:55:45 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 19:53:44 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	single_philo(t_data *data, t_philo philo)
{
	if (philo.total == 1)
	{
		usleep(philo.ttd * 1000);
		philo_died(data, philo);
	}
}

void	check_for_dead(t_data *data, int first, int second)
{
	pthread_mutex_lock(&(data->mutex_died));
	if (data->died == 1)
	{
		pthread_mutex_unlock(&(data->mutex_died));
		if (first != -1)
			pthread_mutex_unlock(&(data->mutex_forks[first]));
		if (second != -1)
			pthread_mutex_unlock(&(data->mutex_forks[second]));
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&(data->mutex_died));
}

void	philo_died(t_data *data, t_philo philo)
{
	message(data, philo, "died");
	pthread_mutex_lock(&(data->mutex_died));
	data->died = 1;
	pthread_mutex_unlock(&(data->mutex_died));
	pthread_exit(NULL);
}

void	philo_finished(t_data *data)
{
	pthread_mutex_lock(&(data->mutex_died));
	data->died = 1;
	pthread_mutex_unlock(&(data->mutex_died));
	pthread_exit(NULL);
}

void	about_to_die(t_data *data, t_philo philo)
{
	while ((int)(get_time() - philo.last_meal) < philo.ttd)
	{
		check_for_dead(data, -1, -1);
		usleep(100);
	}
	philo_died(data, philo);
}

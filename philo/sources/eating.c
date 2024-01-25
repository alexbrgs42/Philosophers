/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:53:28 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 13:35:01 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	take_fork(t_data *data, t_philo philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&(data->mutex_forks[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[first_fork]));
	data->forks[first_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var[first_fork]));
	message(data, philo, "has taken a fork", 1);
	check_for_dead(data, first_fork, -1);
	check_my_death(data, philo, first_fork, -1);
	pthread_mutex_lock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
	data->forks[second_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
	check_my_death(data, philo, first_fork, second_fork);
	message(data, philo, "is eating", 1);
	ft_usleep(data, philo.tte, first_fork, second_fork);
	pthread_mutex_unlock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
	data->forks[second_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
	pthread_mutex_unlock(&(data->mutex_forks[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[first_fork]));
	data->forks[first_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var[first_fork]));
}

int	count_free_forks(t_data *data, int first_fork, int second_fork)
{
	int	state;

	state = 0;
	pthread_mutex_lock(&(data->mutex_forks_var[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
	state = data->forks[first_fork] + data->forks[second_fork];
	if (state == 2)
	{
		data->forks[first_fork] = 0;
		data->forks[second_fork] = 0;
	}
	pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
	pthread_mutex_unlock(&(data->mutex_forks_var[first_fork]));
	return (state);
}

void	philo_pair(t_data *data, t_philo philo)
{
	int	state;
	int	loop;
	int	left;

	state = 0;
	loop = 0;
	left = philo.nb_philo - 1;
	if (left < 0)
		left = philo.total - 1;
	while (state != 2)
	{
		state = count_free_forks(data, philo.nb_philo, left);
		if ((int)(get_time() - philo.last_meal) >= philo.ttd)
			philo_died(data, philo);
		if (state != 2)
			usleep(10);
		if (loop++ > 50 && state == 1)
			state = check_loop(data, philo.nb_philo, left);
	}
	take_fork(data, philo, philo.nb_philo, left);
}

void	philo_impair(t_data *data, t_philo philo)
{
	int	state;
	int	loop;

	state = 0;
	loop = 0;
	while (state != 2)
	{
		state = count_free_forks(data, philo.nb_philo - 1, philo.nb_philo);
		if ((int)(get_time() - philo.last_meal) >= philo.ttd)
			philo_died(data, philo);
		if (state != 2)
			usleep(5);
		if (loop++ > 50 && state == 1)
			state = check_loop(data, philo.nb_philo - 1, philo.nb_philo);
	}
	take_fork(data, philo, philo.nb_philo - 1, philo.nb_philo);
}

void	eating(t_data *data, t_philo philo)
{
	single_philo(data, philo);
	if (philo.nb_philo % 2 == 0)
		philo_pair(data, philo);
	else
	{
		usleep(20);
		philo_impair(data, philo);
	}
}

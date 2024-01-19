/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:53:28 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/19 14:59:51 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	take_fork(t_data *data, t_philo philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&(data->mutex_forks[first_fork]));
	message(data, philo, "has taken a fork");
	pthread_mutex_lock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
	data->forks[second_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
	message(data, philo, "is eating");
	usleep(philo.tte * 1000);
	pthread_mutex_unlock(&(data->mutex_forks[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[first_fork]));
	data->forks[first_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var[first_fork]));
	pthread_mutex_unlock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
	data->forks[second_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
}

void	philo_pair(t_data *data, t_philo philo)
{
	int	state;
	int	left;

	state = 0;
	left = philo.nb_philo - 1;
	if (left < 0)
		left = philo.total - 1;
	while (state != 2)
	{
		pthread_mutex_lock(&(data->mutex_forks_var[philo.nb_philo]));
		pthread_mutex_lock(&(data->mutex_forks_var[left]));
		state = data->forks[philo.nb_philo] + data->forks[left];
		if (state == 2)
		{
			data->forks[philo.nb_philo] = 0;
			data->forks[left] = 0;
		}
		pthread_mutex_unlock(&(data->mutex_forks_var[left]));
		pthread_mutex_unlock(&(data->mutex_forks_var[philo.nb_philo]));
		if (get_time() - philo.last_meal >= philo.ttd)
			philo_died(data, philo);
		if (state != 2)
			usleep(5);
	}
	take_fork(data, philo, philo.nb_philo, left);
}

void	philo_impair(t_data *data, t_philo philo)
{
	int	state;
	int	left;

	state = 0;
	left = philo.nb_philo - 1;
	while (state != 2)
	{
		pthread_mutex_lock(&(data->mutex_forks_var[philo.nb_philo - 1]));
		pthread_mutex_lock(&(data->mutex_forks_var[philo.nb_philo]));
		state = data->forks[philo.nb_philo - 1] + data->forks[philo.nb_philo];
		if (state == 2)
		{
			data->forks[philo.nb_philo - 1] = 0;
			data->forks[philo.nb_philo] = 0;
		}
		pthread_mutex_unlock(&(data->mutex_forks_var[philo.nb_philo]));
		pthread_mutex_unlock(&(data->mutex_forks_var[philo.nb_philo - 1]));
		if (get_time() - philo.last_meal >= philo.ttd)
			philo_died(data, philo);
		if (state != 2)
			usleep(5);
	}
	take_fork(data, philo, philo.nb_philo - 1, philo.nb_philo);
}

void	eating(t_data *data, t_philo philo)
{
	if (philo.nb_philo % 2 == 0)
	{
		philo_pair(data, philo);
	}
	else
	{
		usleep(10);
		philo_impair(data, philo);
	}
}

void	increment_number_meals(t_data *data, t_philo philo, int *i)
{
	int	max_meals;

	pthread_mutex_lock(&(data->mutex_meals));
	max_meals = data->number_of_times_each_philosopher_must_eat;
	pthread_mutex_unlock(&(data->mutex_meals));
	if (max_meals == -1)
		return ;
	else
		(*i)++;
	if (*i == max_meals)
	{
		pthread_mutex_lock(&(data->mutex_meals));
		data->philo_done += 1;
		pthread_mutex_unlock(&(data->mutex_meals));
	}
	pthread_mutex_lock(&(data->mutex_meals));
	if (data->philo_done == data->number_of_philosophers)
	{
		pthread_mutex_unlock(&(data->mutex_meals));
		philo_finished(data, philo);
	}
	pthread_mutex_unlock(&(data->mutex_meals));
}

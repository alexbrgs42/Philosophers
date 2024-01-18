/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 18:53:28 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/18 20:20:24 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	take_fork(t_data *data, t_philo philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&(data->mutex_forks[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var));
	data->forks[first_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var));
	message(data, "has taken a fork", philo.nb_philo);
	pthread_mutex_lock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var));
	data->forks[second_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var));
	message(data, "is eating", philo.nb_philo);
	usleep(philo.tte * 1000);
	pthread_mutex_unlock(&(data->mutex_forks[first_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var));
	data->forks[first_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var));
	pthread_mutex_unlock(&(data->mutex_forks[second_fork]));
	pthread_mutex_lock(&(data->mutex_forks_var));
	data->forks[second_fork] = 1;
	pthread_mutex_unlock(&(data->mutex_forks_var));
}

void	philo_pair(t_data *data, t_philo philo)
{
	int	state;
	int	left;

	state = 0;
	left = philo.nb_philo - 1;
	if (left < 0)
		left = philo.total - 1;
	while (state != 1)
	{
		if (get_time() - philo.last_meal >= philo.ttd)
			philo_died(data, philo);
		pthread_mutex_lock(&(data->mutex_forks_var));
		state = data->forks[philo.nb_philo];
		if (state == 1)
			data->forks[philo.nb_philo] = 0;
		pthread_mutex_unlock(&(data->mutex_forks_var));
		if (state == 0)
			usleep(10);
		check_for_dead(data);
	}
	take_fork(data, philo, philo.nb_philo, left);
}

void	philo_impair(t_data *data, t_philo philo)
{
	int	state;

	state = 0;
	while (state != 1)
	{
		if (get_time() - philo.last_meal >= philo.ttd)
			philo_died(data, philo);
		pthread_mutex_lock(&(data->mutex_forks_var));
		state = data->forks[philo.nb_philo - 1];
		if (state == 1)
			data->forks[philo.nb_philo - 1] = 0;
		pthread_mutex_unlock(&(data->mutex_forks_var));
		if (state == 0)
			usleep(10);
	}
	take_fork(data, philo, philo.nb_philo - 1, philo.nb_philo);
}

void	eating(t_data *data, t_philo philo)
{
	if (philo.nb_philo % 2 == 0)
		philo_pair(data, philo);
	else
		philo_impair(data, philo);
	check_for_dead(data);
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

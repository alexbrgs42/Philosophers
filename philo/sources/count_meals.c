/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_meals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:59:33 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 13:36:44 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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
		message(data, philo, "is sleeping", 0);
		philo_finished(data);
	}
	pthread_mutex_unlock(&(data->mutex_meals));
}

void	check_my_death(t_data *data, t_philo philo, int first_fork,
	int second_fork)
{
	if ((int)(get_time() - philo.last_meal) >= philo.ttd)
	{
		if (second_fork != -1)
			pthread_mutex_unlock(&(data->mutex_forks[second_fork]));
		pthread_mutex_unlock(&(data->mutex_forks[first_fork]));
		philo_died(data, philo);
	}
}

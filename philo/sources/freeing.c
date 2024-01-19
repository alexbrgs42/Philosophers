/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 21:05:49 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/19 14:00:20 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	first_free(t_data *data, int pr, int st, int fo)
{
	int	i;

	i = data->number_of_philosophers;
	while (--i >= 0)
		pthread_mutex_destroy(&(data->mutex_forks[i]));
	if (pr == 1)
		pthread_mutex_destroy(&(data->mutex_printf));
	if (st == 1)
		pthread_mutex_destroy(&(data->mutex_struct));
	if (fo == 1)
		pthread_mutex_destroy(&(data->mutex_died));
	pthread_mutex_destroy(&(data->mutex_begin));
	free(data->forks);
	free(data->mutex_forks);
	return (0);
}

int	final_free(t_data *data, int nb_thread, int join, int return_val)
{
	int	i;

	i = 0;
	first_free(data, 1, 1, 1);
	while (i < data->number_of_philosophers)
		pthread_mutex_destroy(&(data->mutex_forks_var[i++]));
	free(data->mutex_forks_var);
	pthread_mutex_destroy(&(data->mutex_meals));
	i = 0;
	if (join == 1)
	{
		while (i < nb_thread)
			pthread_join(data->philo[i++], NULL);
	}
	free(data->philo);
	return (return_val);
}

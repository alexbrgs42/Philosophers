/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 21:05:49 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 13:07:45 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	first_free(t_data *data, int nb)
{
	int	i;

	i = data->number_of_philosophers;
	while (--i >= 0)
		pthread_mutex_destroy(&(data->mutex_forks[i]));
	if (nb-- > 1)
		pthread_mutex_destroy(&(data->mutex_printf));
	if (nb-- > 1)
		pthread_mutex_destroy(&(data->mutex_struct));
	if (nb-- > 1)
		pthread_mutex_destroy(&(data->mutex_died));
	if (nb-- > 1)
		pthread_mutex_destroy(&(data->mutex_meals));
	if (nb-- > 1)
		pthread_mutex_destroy(&(data->mutex_create));
	pthread_mutex_destroy(&(data->mutex_begin));
	free(data->forks);
	free(data->mutex_forks);
	return (0);
}

int	final_free(t_data *data, int nb_thread, int join, int return_val)
{
	int	i;

	i = 0;
	if (join == 1)
	{
		data->creation = 0;
		pthread_mutex_unlock(&(data->mutex_create));
		while (i < nb_thread)
			pthread_join(data->philo[i++], NULL);
	}
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_destroy(&(data->mutex_forks_var[i++]));
	free(data->mutex_forks_var);
	free(data->philo);
	first_free(data, 6);
	return (return_val);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:59:24 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 12:43:40 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	free_parent_thread(t_data *data, t_philo_parent *philo)
{
	free_data(data);
	close_semaphores(philo, NB_SEM);
	exit(1);
}

void	free_data(t_data *data)
{
	free(data->pid);
	sem_close(data->sem_parent_struct);
	sem_close(data->sem_init);
	sem_close(data->sem_death_var);
	sem_close(data->sem_meals);
	sem_close(data->sem_nb_meals);
	sem_close(data->sem_printf);
	sem_close(data->sem_forks);
	sem_close(data->sem_last_meal);
	sem_close(data->sem_takes_forks);
	sem_close(data->sem_death);
	sem_close(data->sem_increment);
	sem_close(data->sem_start);
}

int	close_semaphores(t_philo_parent *philo, int i)
{
	if (i-- > 0)
		sem_close(philo->sem_parent_struct);
	if (i-- > 0)
		sem_close(philo->sem_death_var);
	if (i-- > 0)
		sem_close(philo->sem_meals);
	if (i-- > 0)
		sem_close(philo->sem_printf);
	if (i-- > 0)
		sem_close(philo->sem_forks);
	if (i-- > 0)
		sem_close(philo->sem_last_meal);
	if (i-- > 0)
		sem_close(philo->sem_death);
	if (i-- > 0)
		sem_close(philo->sem_takes_forks);
	if (i-- > 0)
		sem_close(philo->sem_nb_meals);
	if (i-- > 0)
		sem_close(philo->sem_increment);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:21:13 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 16:58:40 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	final_free(t_data *data)
{
	free_semaphores(data, NB_SEM);
	free(data->pid);
}

int	free_semaphores(t_data *data, int i)
{
	if (i-- > 0)
	{
		sem_close(data->sem_death_var);
		sem_unlink(SEM_DEATH_VAR);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_init);
		sem_unlink(SEM_INIT);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_meals);
		sem_unlink(SEM_MEALS);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_printf);
		sem_unlink(SEM_PRINTF);
	}
	free_semaphores_bis(data, i);
	return (0);
}

void	free_semaphores_bis(t_data *data, int i)
{
	if (i-- > 0)
	{
		sem_close(data->sem_forks);
		sem_unlink(SEM_FORKS);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_last_meal);
		sem_unlink(SEM_LAST_MEAL);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_parent_struct);
		sem_unlink(SEM_PARENT_STRUCT);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_takes_forks);
		sem_unlink(SEM_TAKES_FORKS);
	}
	free_semaphores_bis_bis(data, i);
}

void	free_semaphores_bis_bis(t_data *data, int i)
{
	if (i-- > 0)
	{
		sem_close(data->sem_death);
		sem_unlink(SEM_DEATH);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_nb_meals);
		sem_unlink(SEM_NB_MEALS);
	}
	if (i-- > 0)
	{
		sem_close(data->sem_increment);
		sem_unlink(SEM_INCREMENT);
	}
}

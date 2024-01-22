/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:21:13 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/22 11:39:40 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void    final_free(t_data *data)
{
	free_semaphores(data, NB_SEM);
	free(data->pid);
    // ...
}

int	free_semaphores(t_data *data, int i)
{
	if (i-- > 0)
	{
		sem_close(data->sem_died);
		sem_unlink("/sem_died");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_init);
		sem_unlink("/sem_init");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_meals);
		sem_unlink("/sem_meals");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_printf);
		sem_unlink("/sem_printf");
	}
	free_semaphores_bis(data, i);
	return (0);
}

void	free_semaphores_bis(t_data *data, int i)
{
	if (i-- > 0)
	{
		sem_close(data->sem_forks);
		sem_unlink("/sem_forks");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_last_meal);
		sem_unlink("/sem_last_meal");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_free_forks);
		sem_unlink("/sem_free_forks");
	}
	if (i-- > 0)
	{
		sem_close(data->sem_parent_struct);
		sem_unlink("/sem_parent_struct");
	}
}

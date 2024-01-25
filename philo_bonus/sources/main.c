/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:39:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 13:51:54 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	set_semaphore_state(void)
{
	sem_unlink(SEM_DEATH_VAR);
	sem_unlink(SEM_INIT);
	sem_unlink(SEM_MEALS);
	sem_unlink(SEM_PRINTF);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_LAST_MEAL);
	sem_unlink(SEM_PARENT_STRUCT);
	sem_unlink(SEM_TAKES_FORKS);
	sem_unlink(SEM_DEATH);
	sem_unlink(SEM_NB_MEALS);
	sem_unlink(SEM_INCREMENT);
	sem_unlink(SEM_START);
}

void	thread_failed(t_data data, int *i)
{
	*i = data.number_of_philosophers;
	sem_post(data.sem_death);
}

void	waiting(t_data *data, int i)
{
	if (i == data->number_of_philosophers + 1)
	{
		i = 0;
		while (i < data->number_of_philosophers && data->pid[i] != -1)
			kill(data->pid[i++], SIGKILL);
		sem_post(data->sem_start);
	}
	else
	{
		sem_post(data->sem_start);
		i = 0;
		while (i < data->number_of_philosophers && data->pid[i] != -1)
			waitpid(data->pid[i++], NULL, 0);
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		i;

	if (argc == 5 || argc == 6)
	{
		if (init_data(&data, argv) == 0)
			return (1);
		i = 0;
		sem_wait(data.sem_start);
		while (i < data.number_of_philosophers && ((i == 0)
				|| data.pid[i - 1] != 0))
		{
			data.pid[i] = fork();
			if (data.pid[i] == 0)
				start(&data, i);
			else if (data.pid[i] == -1)
				thread_failed(data, &i);
			i++;
		}
		waiting(&data, i);
		final_free(&data);
	}
	return (0);
}

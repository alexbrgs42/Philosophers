/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:55:31 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 20:38:36 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int	init_data(t_data *data, char *argv[])
{
	int	i;

	i = 0;
	if (init_numbers(data, argv) == 0)
		return (0);
	if (init_semaphores(data) == 0)
		return (0);
	data->pid = malloc(data->number_of_philosophers * sizeof(int));
	if (data->pid == NULL)
		return (free_semaphores(data, NB_SEM));
	while (i < data->number_of_philosophers)
		data->pid[i++] = 0;
	sem_wait(data->sem_death);
	i = 0;
	while (i++ < data->number_of_philosophers)
		sem_wait(data->sem_meals);
	return (1);
}

int	nb_digits(int i)
{
	int	count;

	count = 0;
	if (i < 10)
		return (1);
	while (i != 0)
	{
		count++;
		i = i / 10;
	}
	return (count);
}

int	init_semaphores(t_data *data)
{
	set_semaphore_state();
	data->sem_death_var = sem_open(SEM_DEATH_VAR, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_death_var == SEM_FAILED)
		return (0);
	data->sem_init = sem_open(SEM_INIT, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_init == SEM_FAILED)
		return (free_semaphores(data, 1));
	data->sem_meals = sem_open(SEM_MEALS, O_CREAT | O_EXCL, 0644,
			data->number_of_philosophers);
	if (data->sem_meals == SEM_FAILED)
		return (free_semaphores(data, 2));
	data->sem_printf = sem_open(SEM_PRINTF, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_printf == SEM_FAILED)
		return (free_semaphores(data, 3));
	data->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644,
			data->number_of_philosophers);
	if (data->sem_forks == SEM_FAILED)
		return (free_semaphores(data, 4));
	data->sem_last_meal = sem_open(SEM_LAST_MEAL, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_last_meal == SEM_FAILED)
		return (free_semaphores(data, 5));
	return (init_semaphores_bis(data));
}

int	init_semaphores_bis(t_data *data)
{
	data->sem_parent_struct = sem_open(SEM_PARENT_STRUCT, O_CREAT | O_EXCL,
			0644, 1);
	if (data->sem_parent_struct == SEM_FAILED)
		return (free_semaphores(data, 6));
	data->sem_takes_forks = sem_open(SEM_TAKES_FORKS, O_CREAT | O_EXCL,
			0644, 1);
	if (data->sem_takes_forks == SEM_FAILED)
		return (free_semaphores(data, 7));
	data->sem_death = sem_open(SEM_DEATH, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_death == SEM_FAILED)
		return (free_semaphores(data, 8));
	data->sem_nb_meals = sem_open(SEM_NB_MEALS, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_nb_meals == SEM_FAILED)
		return (free_semaphores(data, 9));
	data->sem_increment = sem_open(SEM_INCREMENT, O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_increment == SEM_FAILED)
		return (free_semaphores(data, 10));
	return (1);
}

int	init_numbers(t_data *data, char *argv[])
{
	int	return_val;

	return_val = 0;
	data->start_time = get_time();
	return_val += ft_atoi(argv[1], &(data->number_of_philosophers));
	return_val += ft_atoi(argv[2], &(data->time_to_die));
	return_val += ft_atoi(argv[3], &(data->time_to_eat));
	return_val += ft_atoi(argv[4], &(data->time_to_sleep));
	if (argv[5] != NULL)
		return_val += ft_atoi(argv[5],
				&(data->number_of_times_each_philosopher_must_eat));
	else
	{
		return_val++;
		data->number_of_times_each_philosopher_must_eat = -1;
	}
	if (data->number_of_philosophers == 0 || data->time_to_die == 0
		|| data->time_to_eat == 0 || data->time_to_sleep == 0
		|| data->number_of_times_each_philosopher_must_eat == 0)
		return (0);
	return (return_val == 5);
}

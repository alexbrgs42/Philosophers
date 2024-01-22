/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:55:31 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/22 17:30:05 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int init_data(t_data *data, char *argv[])
{
	int	i;

	i = 0;
	if (init_numbers(data, argv) == 0)
		return (printf("help3\n"));
	if (init_semaphores(data) == 0)
		return (printf("help4\n"));
	data->pid = malloc(data->number_of_philosophers * sizeof(int));
	if (data->pid == NULL)
		return (free_semaphores(data, NB_SEM));
	while (i < data->number_of_philosophers)
		data->pid[i++] = 0;
	return (1);
}

int init_semaphores(t_data *data)
{
	data->sem_died = sem_open("/sem_died", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_died == SEM_FAILED)
		return (0);
	data->sem_init = sem_open("/sem_init", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_init == SEM_FAILED)
		return (free_semaphores(data, 1));
	data->sem_meals = sem_open("/sem_meals", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_meals == SEM_FAILED)
		return (free_semaphores(data, 2));
	data->sem_printf = sem_open("/sem_printf", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_printf == SEM_FAILED)
		return (free_semaphores(data, 3));
	data->sem_forks = sem_open("/sem_forks", O_CREAT | O_EXCL, 0644,
		data->number_of_philosophers);
	if (data->sem_forks == SEM_FAILED)
		return (free_semaphores(data, 4));
	data->sem_last_meal = sem_open("/sem_last_meal", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_last_meal == SEM_FAILED)
		return (free_semaphores(data, 5));
	data->sem_free_forks = sem_open("/sem_free_forks", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_free_forks == SEM_FAILED)
		return (free_semaphores(data, 6));
	data->sem_parent_struct = sem_open("/sem_parent_struct", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem_parent_struct == SEM_FAILED)
		return (free_semaphores(data, 7));
	return (1);
}

int init_numbers(t_data *data, char *argv[])
{
	int	return_val;

	return_val = 0;
	data->shared_died = 0;
	data->shared_philo_done = 0;
	data->start_time = get_time();
	return_val += ft_atoi(argv[1], &(data->number_of_philosophers));
	return_val += ft_atoi(argv[2], &(data->time_to_die));
	return_val += ft_atoi(argv[3], &(data->time_to_eat));
	return_val += ft_atoi(argv[4], &(data->time_to_sleep));
	data->shared_free_forks = data->number_of_philosophers;
	if (argv[5] != NULL)
		return_val += ft_atoi(argv[5],
				&(data->number_of_times_each_philosopher_must_eat));
	else
	{
		return_val++;
		data->shared_philo_done = -1;
		data->number_of_times_each_philosopher_must_eat = -1;
	}
	if (data->number_of_philosophers == 0 || data->time_to_die == 0
		|| data->time_to_eat == 0 || data->time_to_sleep == 0
		|| data->number_of_times_each_philosopher_must_eat == 0)
		return (0);
	return (return_val == 5);
}

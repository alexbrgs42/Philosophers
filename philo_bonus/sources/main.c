/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:39:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 00:11:45 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

// void	main_death_routine(void *data_void)
// {
// 	t_data	*data;

// 	sem_wait((t_data *)data->sem_init);
// 	data = (t_data *)data_void;
// 	sem_post((t_data *)data->sem_init);
	
// }

// void	meals_routine(void *data_void)
// {
// 	int	start_time;
// 	int	tte;

// 	sem_wait(((t_data *)data_void)->sem_init);
// 	start_time = ((t_data *)data_void)->start_time;
// 	tte = ((t_data *)data_void)->time_to_eat;
// 	sem_post(((t_data *)data_void)->sem_init);
// 	while (get_time() - start_time < 1000)
// 		;
// 	usleep(tte);
// 	sem_wait(((t_data *)data_void)->sem_meals);
// }

int main(int argc, char *argv[])
{
	t_data	data;
	int		i;

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
	if (argc == 5 || argc == 6)
	{
		if (init_data(&data, argv) == 0)
			return (1);
		// bloquer toutes les semaphores
		sem_wait(data.sem_death);
		i = 0;
		while (i++ < data.number_of_philosophers)
			sem_wait(data.sem_meals);
		i = 0;
		while (i < data.number_of_philosophers)
		{
			data.pid[i] = fork();
			if (data.pid[i] == 0)
				start(&data, i);
			else if (data.pid[i] == -1)
				printf("help\n"); // ...
			else
				i++;
		}
		i = 0;
		while (i < data.number_of_philosophers)
			waitpid(data.pid[i++], NULL, 0);
		final_free(&data);
    }
	return (0);
}

// once everyone ate, unlock death
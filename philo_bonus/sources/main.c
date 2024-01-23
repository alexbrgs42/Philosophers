/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:39:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/23 20:35:14 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	main_death_routine(void *data_void)
{
	t_data	*data;

	sem_wait((t_data *)data->sem_init);
	data = (t_data *)data_void;
	sem_post((t_data *)data->sem_init);
	
}

int main(int argc, char *argv[])
{
	t_data	data;
	int		i;
	char	 *str;

	sem_unlink(SEM_DEATH_VAR);
	sem_unlink(SEM_INIT);
	sem_unlink(SEM_MEALS);
	sem_unlink(SEM_PRINTF);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_LAST_MEAL);
	sem_unlink(SEM_PARENT_STRUCT);
	sem_unlink(SEM_TAKES_FORKS);
	if (argc == 5 || argc == 6)
	{
		if (init_data(&data, argv) == 0)
			return (1);
		i = 0;
		while (i < data.number_of_philosophers)
		{
			str = ft_strjoin(SEM_DEATH, i);
			if (str == NULL)
				return (-1);
			sem_unlink(str);
			free(str);
			i++;
		}
		// bloquer toutes les semaphores
		i = 0;
		while (i < data.number_of_philosophers)
			sem_wait(data.sem_death[i++]);
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
		pthread_create(&(data.death_thread), NULL, main_death_routine, (void *) (&data));
		i = 0;
		while (i < data.number_of_philosophers)
			waitpid(data.pid[i++], NULL, 0);
		printf("done\n");
		final_free(&data);
    }
	return (0);
}

// once everyone ate, unlock death
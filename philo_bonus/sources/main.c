/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:39:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/22 16:03:22 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int main(int argc, char *argv[])
{
	t_data	data;
	int		i;
	int		status;

	i = 0;
	status = 0;
	sem_unlink("/sem_died");
	sem_unlink("/sem_init");
	sem_unlink("/sem_meals");
	sem_unlink("/sem_printf");
	sem_unlink("/sem_forks");
	sem_unlink("/sem_last_meal");
	sem_unlink("/sem_free_forks");
	sem_unlink("/sem_parent_struct");
	if (argc == 5 || argc == 6)
	{
		if (init_data(&data, argv) == 0)
			return (1);
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
		{
			waitpid(data.pid[i++], &status, 0);
			if (status == -2)
			{
				while (i < data.number_of_philosophers)
					kill(data.pid[i++], SIGTERM);
			}
		}
		printf("done\n");
		final_free(&data);
    }
	return (0);
}

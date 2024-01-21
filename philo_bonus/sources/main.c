/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:39:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/21 13:31:04 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int main(int argc, char *argv[])
{
	t_data	data;
	int		i;

	i = 0;
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
			waitpid(data.pid[i++], NULL, 0);
		final_free(&data);
    }
	return (0);
}

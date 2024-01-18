/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 10:42:01 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/18 20:22:45 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *data_void)
{
	t_data	*data;
	t_philo	philo;
	int		i;

	i = 0;
	pthread_mutex_lock(&(((t_data *) data_void)->mutex_struct));
	data = (t_data *) data_void;
	pthread_mutex_unlock(&(data->mutex_struct));
	init_t_philo(data, &philo);
	pthread_mutex_lock(&(data->mutex_begin));
	if (philo.nb_philo == 0)
	{
		usleep(1000);
		pthread_mutex_lock(&(data->mutex_struct));
		data->start_time = get_time();
		pthread_mutex_unlock(&(data->mutex_struct));
	}
	philo.last_meal = data->start_time;
	pthread_mutex_unlock(&(data->mutex_begin));
	while (1)
	{
		eating(data, philo);
		increment_number_meals(data, philo, &i);
		philo.last_meal = get_time() - philo.tte;
		message(data, "is sleeping", philo.nb_philo);
		if (get_time() - philo.last_meal + philo.tts > philo.ttd)
			about_to_die(data, philo);
		check_for_dead(data);
		usleep(philo.tts* 1000);
		check_for_dead(data);
		message(data, "is thinking", philo.nb_philo);
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		i;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		if (init_data(&data, argv) == 0)
			return (1);
		while (i < data.number_of_philosophers)
			if (pthread_create(&(data.philo[i++]), NULL,
					&routine, (void *)(&data)) != 0)
				final_free(&data, i - 1, 1, 0);
		i = 0;
		while (i < data.number_of_philosophers)
			pthread_join(data.philo[i++], NULL);
		printf("Done\n");
		final_free(&data, i, 0, 1);
	}
	return (0);
}

/*

// cas particuliers des args :
argv[1] -> nb_philo > 0 et si = 1 meurt apres usleep(ttd)
argv[2] -> ttd pas negatif ou nul
argv[3] -> tte pas negatif ou nul
argv[4] -> tts pas negatif ou nul
argv[5] -> max_meals pas negatif ou nul

// flags du Makefile a corriger

// faire un mutex par var forks[i]

// premier et denier ok ?

*/
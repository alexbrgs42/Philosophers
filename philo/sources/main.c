/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 10:42:01 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 20:46:52 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_loop(t_data *data, int first_fork, int second_fork)
{
	int	state;

	state = 0;
	pthread_mutex_lock(&(data->mutex_forks_var[first_fork]));
	state = data->forks[first_fork];
	if (state == 1)
		data->forks[first_fork] = 0;
	pthread_mutex_unlock(&(data->mutex_forks_var[first_fork]));
	if (state == 0)
	{
		pthread_mutex_lock(&(data->mutex_forks_var[second_fork]));
		data->forks[second_fork] = 0;
		pthread_mutex_unlock(&(data->mutex_forks_var[second_fork]));
	}
	return (2);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned)s1[i] - (unsigned)s2[i]);
}

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
	while (get_time() - philo.start_time < 1000)
		;
	single_philo(data, philo);
	while (1)
	{
		eating(data, philo);
		increment_number_meals(data, philo, &i);
		philo.last_meal = get_time() - philo.tte;
		message(data, philo, "is sleeping");
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			about_to_die(data, philo);
		ft_usleep(data, philo.tts, -1, -1);
		message(data, philo, "is thinking");
		usleep(20);
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
		final_free(&data, i, 0, 1);
	}
	return (0);
}

/*
Arreter d'afficher des messages avec un booleen des qu'un philo meurt
ou qu'ils ont tous finis de manger
*/

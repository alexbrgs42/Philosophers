/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 10:42:01 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/17 23:09:21 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	message(t_data *data, char *str, int nb_philo)
{
	pthread_mutex_lock(&(data->mutex_printf));
	printf("%zu %d %s\n", get_time() - data->start_time, nb_philo, str);
	pthread_mutex_unlock(&(data->mutex_printf));
}

// pas bloquer la fourchette donc faire dans ce sens ou l'autre
// si pair, prendre celle de droite en premier, sinon celle de gauche en premier
void	philo_pair(t_data *data, int nb_philo, int total)
{
	int	left;

	left = nb_philo - 1;
	if (nb_philo == 0)
		left = total - 1;
	pthread_mutex_lock(&(data->mutex_forks[nb_philo]));
	message(data, "has taken a fork", nb_philo);
	pthread_mutex_lock(&(data->mutex_forks[left]));
	message(data, "is eating", nb_philo);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&(data->mutex_forks[nb_philo]));
	pthread_mutex_unlock(&(data->mutex_forks[left]));
	message(data, "is thinking", nb_philo);
}

void	philo_impair(t_data *data, int nb_philo)
{
	pthread_mutex_lock(&(data->mutex_forks[nb_philo - 1]));
	message(data, "has taken a fork", nb_philo);
	pthread_mutex_lock(&(data->mutex_forks[nb_philo]));
	message(data, "is eating", nb_philo);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&(data->mutex_forks[nb_philo - 1]));
	pthread_mutex_unlock(&(data->mutex_forks[nb_philo]));
}

void	eating(t_data *data, int nb_philo)
{
	int	total;

	pthread_mutex_lock(&(data->mutex_struct));
	total = data->number_of_philosophers;
	pthread_mutex_unlock(&(data->mutex_struct));
	if (nb_philo % 2 == 0) // autre cas pour philo 0 et dernier (soucis last fourchette)
		philo_pair(data, nb_philo, total);
	else
		philo_impair(data, nb_philo);
	pthread_mutex_lock(&(data->mutex_struct));
	if (data->died == 1)
		pthread_exit(NULL);
	pthread_mutex_unlock(&(data->mutex_struct));
}
// quand mange suffisamment, il faut continuer
void	*routine(void *data_void)
{
	t_data	*data;
	int		nb_philo;

	pthread_mutex_lock(&(((t_data *) data_void)->mutex_struct));
	data = (t_data *) data_void;
	nb_philo = data->counter++;
	pthread_mutex_unlock(&(data->mutex_struct));
	pthread_mutex_lock(&(data->mutex_printf));
	pthread_mutex_unlock(&(data->mutex_printf));
	while (1)
	{
		eating(data, nb_philo);
		message(data, "is sleeping", nb_philo);
		usleep(data->time_to_sleep * 1000);
		message(data, "is thinking", nb_philo);
	}
}

// flags makefile fonctionnent pas
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
			if (pthread_create(&(data.philo[i++]), NULL, &routine, (void *)(&data)) != 0)
				final_free(&data, i - 1, 1, 0);
		i = 0;
		while (i < data.number_of_philosophers)
			pthread_join(data.philo[i++], NULL);
		printf("Done\n");
		final_free(&data, i, 0, 1);
	}
	return (0);
}

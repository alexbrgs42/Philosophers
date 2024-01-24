/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:25:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 20:36:15 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	start_routines(t_data *data, t_philo_parent *philo_parent,
	int nb_philo)
{
	if (init_philo(data, philo_parent, nb_philo) == 0)
		free_parent_thread(data, philo_parent);
	philo_parent->shared_last_meal = philo_parent->start_time + (size_t)1000;
	if (pthread_create(&(philo_parent->philo_thread), NULL, routine,
			(void *)philo_parent) != 0)
		printf("help1\n");
	if (pthread_create(&(philo_parent->death_thread), NULL, death_routine,
			(void *)philo_parent) != 0)
		printf("help2\n");
	if (pthread_create(&(philo_parent->meals_thread), NULL, meals_routine,
			(void *)philo_parent) != 0)
		printf("help3\n");
}

void	end_routines(t_data *data, t_philo_parent *philo_parent,
	int nb_philo, size_t start_time)
{
	if (philo_parent->shared_died == 0)
	{
		if (philo_parent->nb_meals == 0)
			message(philo_parent, "died", start_time);
		philo_parent->shared_died = 1;
		sem_post(philo_parent->sem_death);
	}
	sem_post(philo_parent->sem_nb_meals);
	sem_post(philo_parent->sem_death_var);
	pthread_join(philo_parent->death_thread, NULL);
	pthread_join(philo_parent->philo_thread, NULL);
	while (nb_philo-- > 0)
		sem_post(philo_parent->sem_meals);
	pthread_join(philo_parent->meals_thread, NULL);
	free_parent_thread(data, philo_parent);
}

void	start(t_data *data, int nb_philo)
{
	t_philo_parent	philo_parent;
	size_t			last_meal;
	size_t			start_time;

	start_routines(data, &philo_parent, nb_philo);
	sem_wait(philo_parent.sem_parent_struct);
	start_time = philo_parent.start_time;
	sem_post(philo_parent.sem_parent_struct);
	while (get_time() - start_time < 1000)
		;
	while (1)
	{
		sem_wait(philo_parent.sem_last_meal);
		last_meal = get_time() - philo_parent.shared_last_meal;
		sem_post(philo_parent.sem_last_meal);
		sem_wait(philo_parent.sem_death_var);
		sem_wait(philo_parent.sem_nb_meals);
		if ((int)last_meal >= philo_parent.ttd || philo_parent.shared_died == 1
			|| philo_parent.nb_meals > 0)
			end_routines(data, &philo_parent, nb_philo, start_time);
		sem_post(philo_parent.sem_nb_meals);
		sem_post(philo_parent.sem_death_var);
		usleep(10);
	}
}

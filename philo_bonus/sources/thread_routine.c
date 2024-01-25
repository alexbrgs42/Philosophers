/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:02:48 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 14:12:12 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	*routine(void *void_philo)
{
	int				i;
	t_philo_child	philo;
	t_philo_parent	*philo_parent;

	i = 0;
	philo_parent = copies_t_philo(&philo, void_philo);
	while (get_time() - philo.start_time < 1000)
		;
	if (philo.total == 1)
		pthread_exit(NULL);
	while (1)
	{
		philo_eats(philo_parent, philo);
		philo.last_meal = get_time() - philo.tte;
		is_anyone_dead(philo_parent, 0, 0);
		if (++i == philo.total_meals)
			philo_has_eaten_enough(philo_parent, philo.total);
		message(philo_parent, "is sleeping", philo.start_time);
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			pthread_exit(NULL);
		usleep(philo.tts * 1000);
		is_anyone_dead(philo_parent, 0, 0);
		message(philo_parent, "is thinking", philo.start_time);
	}
}

void	philo_has_eaten_enough(t_philo_parent *philo_parent, int total)
{
	int	j;

	j = 0;
	sem_wait(philo_parent->sem_increment);
	while (j++ < total)
		sem_post(philo_parent->sem_meals);
	sem_post(philo_parent->sem_increment);
}

void	*death_routine(void *philo)
{
	t_philo_parent	*philo_parent;
	size_t			ttd;
	size_t			start_time;

	sem_wait(((t_philo_parent *)philo)->sem_parent_struct);
	philo_parent = (t_philo_parent *)philo;
	ttd = philo_parent->ttd;
	start_time = philo_parent->start_time;
	if (philo_parent->total == 1)
	{
		sem_post(philo_parent->sem_parent_struct);
		pthread_exit(NULL);
	}
	sem_post(philo_parent->sem_parent_struct);
	while (get_time() - start_time < 1000)
		;
	sem_wait(philo_parent->sem_death);
	sem_wait(philo_parent->sem_death_var);
	philo_parent->shared_died = 1;
	sem_post(philo_parent->sem_death_var);
	sem_post(philo_parent->sem_death);
	pthread_exit(NULL);
}

void	*meals_routine(void *philo_void)
{
	t_philo_parent	*philo_parent;
	int				nb_philo_done;
	int				total;

	nb_philo_done = 0;
	sem_wait(((t_philo_parent *)philo_void)->sem_parent_struct);
	philo_parent = (t_philo_parent *)philo_void;
	total = philo_parent->total;
	sem_post(philo_parent->sem_parent_struct);
	if (total == 1)
		pthread_exit(NULL);
	while (nb_philo_done != total)
	{
		is_anyone_dead(philo_parent, 0, 0);
		sem_wait(philo_parent->sem_meals);
		sem_wait(philo_parent->sem_nb_meals);
		nb_philo_done++;
		sem_post(philo_parent->sem_nb_meals);
		usleep(10);
	}
	sem_wait(philo_parent->sem_nb_meals);
	philo_parent->nb_meals = nb_philo_done;
	sem_post(philo_parent->sem_nb_meals);
	pthread_exit(NULL);
}

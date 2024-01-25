/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:03:21 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/25 13:49:35 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	philo_eats(t_philo_parent *philo_parent, t_philo_child philo)
{
	sem_wait(philo_parent->sem_takes_forks);
	sem_wait(philo_parent->sem_forks);
	is_anyone_dead(philo_parent, 1, 1);
	message(philo_parent, "has taken a fork", philo.start_time);
	sem_wait(philo_parent->sem_forks);
	is_anyone_dead(philo_parent, 2, 1);
	sem_wait(philo_parent->sem_last_meal);
	philo_parent->shared_last_meal = get_time();
	sem_post(philo_parent->sem_last_meal);
	message(philo_parent, "is eating", philo.start_time);
	sem_post(philo_parent->sem_takes_forks);
	usleep(philo.tte * 1000);
	sem_post(philo_parent->sem_forks);
	sem_post(philo_parent->sem_forks);
}

void	did_die(t_philo_parent *philo_parent, int start_time)
{
	message(philo_parent, "died", start_time);
	sem_post(philo_parent->sem_death);
}

void	is_anyone_dead(t_philo_parent *philo_parent, int nb_forks, int nb)
{
	int		last_meal;
	size_t	start_time;
	int		ttd;

	sem_wait(philo_parent->sem_last_meal);
	last_meal = get_time() - philo_parent->shared_last_meal;
	sem_post(philo_parent->sem_last_meal);
	sem_wait(philo_parent->sem_parent_struct);
	ttd = philo_parent->ttd;
	start_time = philo_parent->start_time;
	sem_post(philo_parent->sem_parent_struct);
	sem_wait(philo_parent->sem_death_var);
	if (philo_parent->shared_died == 1 || (int)last_meal >= ttd)
	{
		if ((int)last_meal >= ttd && philo_parent->shared_died == 0)
			did_die(philo_parent, start_time);
		philo_parent->shared_died = 1;
		while (nb_forks-- > 0)
			sem_post(philo_parent->sem_forks);
		if (nb == 1)
			sem_post(philo_parent->sem_takes_forks);
		sem_post(philo_parent->sem_death_var);
		pthread_exit(NULL);
	}
	sem_post(philo_parent->sem_death_var);
}

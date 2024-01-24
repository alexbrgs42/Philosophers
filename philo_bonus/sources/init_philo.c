/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:01:32 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 17:02:11 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int	init_var_philo(t_data *data, t_philo_parent *philo, int nb_philo)
{
	philo->sem_takes_forks = sem_open(SEM_TAKES_FORKS, 0);
	if (philo->sem_takes_forks == SEM_FAILED)
		return (close_semaphores(philo, 7));
	philo->sem_nb_meals = sem_open(SEM_NB_MEALS, 0);
	if (philo->sem_nb_meals == SEM_FAILED)
		return (close_semaphores(philo, 8));
	philo->sem_increment = sem_open(SEM_INCREMENT, 0);
	if (philo->sem_increment == SEM_FAILED)
		return (close_semaphores(philo, 9));
	philo->shared_died = 0;
	philo->nb_philo = nb_philo;
	philo->tte = data->time_to_eat;
	philo->tts = data->time_to_sleep;
	philo->ttd = data->time_to_die;
	philo->total = data->number_of_philosophers;
	philo->total_meals = data->number_of_times_each_philosopher_must_eat;
	philo->nb_meals = 0;
	philo->start_time = data->start_time;
	sem_post(data->sem_init);
	return (1);
}

int	init_philo(t_data *data, t_philo_parent *philo, int nb_philo)
{
	sem_wait(data->sem_init);
	philo->sem_parent_struct = sem_open(SEM_PARENT_STRUCT, 0);
	if (philo->sem_parent_struct == SEM_FAILED)
		return (0);
	philo->sem_death_var = sem_open(SEM_DEATH_VAR, 0);
	if (philo->sem_death_var == SEM_FAILED)
		return (close_semaphores(philo, 1));
	philo->sem_meals = sem_open(SEM_MEALS, 0);
	if (philo->sem_meals == SEM_FAILED)
		return (close_semaphores(philo, 2));
	philo->sem_printf = sem_open(SEM_PRINTF, 0);
	if (philo->sem_printf == SEM_FAILED)
		return (close_semaphores(philo, 3));
	philo->sem_forks = sem_open(SEM_FORKS, 0);
	if (philo->sem_forks == SEM_FAILED)
		return (close_semaphores(philo, 4));
	philo->sem_last_meal = sem_open(SEM_LAST_MEAL, 0);
	if (philo->sem_last_meal == SEM_FAILED)
		return (close_semaphores(philo, 5));
	philo->sem_death = sem_open(SEM_DEATH, 0);
	if (philo->sem_death == SEM_FAILED)
		return (close_semaphores(philo, 6));
	return (init_var_philo(data, philo, nb_philo));
}

t_philo_parent	*copies_t_philo(t_philo_child *philo, void *void_philo)
{
	t_philo_parent	*philo_parent;

	sem_wait(((t_philo_parent *)void_philo)->sem_parent_struct);
	philo_parent = (t_philo_parent *)void_philo;
	philo->nb_philo = philo_parent->nb_philo;
	philo->tte = philo_parent->tte;
	philo->tts = philo_parent->tts;
	philo->ttd = philo_parent->ttd;
	philo->total = philo_parent->total;
	philo->start_time = philo_parent->start_time;
	philo->total_meals = philo_parent->total_meals;
	sem_post(philo_parent->sem_parent_struct);
	return (philo_parent);
}

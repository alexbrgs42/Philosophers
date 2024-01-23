/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:25:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 00:11:14 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	init_var_philo(t_data *data, t_philo_parent *philo, int nb_philo)
{
	philo->shared_died = 0;
	philo->nb_philo = nb_philo;
	philo->tte = data->time_to_eat;
	philo->tts = data->time_to_sleep;
	philo->ttd = data->time_to_die;
	philo->total = data->number_of_philosophers;
	philo->total_meals = data->number_of_times_each_philosopher_must_eat;
	philo->nb_meals = 0;
	philo->start_time = data->start_time;
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
	philo->sem_takes_forks = sem_open(SEM_TAKES_FORKS, 0);
	if (philo->sem_takes_forks == SEM_FAILED)
		return (close_semaphores(philo, 7));
	philo->sem_nb_meals = sem_open(SEM_NB_MEALS, 0);
	if (philo->sem_nb_meals == SEM_FAILED)
		return (close_semaphores(philo, 8));
	init_var_philo(data, philo, nb_philo);
	sem_post(data->sem_init);
	return (1);
}

int	close_semaphores(t_philo_parent *philo, int i)
{
	if (i-- > 0)
		sem_close(philo->sem_parent_struct);
	if (i-- > 0)
		sem_close(philo->sem_death_var);
	if (i-- > 0)
		sem_close(philo->sem_meals);
	if (i-- > 0)
		sem_close(philo->sem_printf);
	if (i-- > 0)
		sem_close(philo->sem_forks);
	if (i-- > 0)
		sem_close(philo->sem_last_meal);
	if (i-- > 0)
		sem_close(philo->sem_death);
	if (i-- > 0)
		sem_close(philo->sem_takes_forks);
	if (i-- > 0)
		sem_close(philo->sem_nb_meals);
	return (0);
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

void	philo_eats(t_philo_parent *philo_parent, t_philo_child philo)
{
	sem_wait(philo_parent->sem_takes_forks);
	sem_wait(philo_parent->sem_forks);
	is_anyone_dead(philo_parent); // here check for death
	message(philo_parent, "has taken a fork", philo.start_time);
	sem_wait(philo_parent->sem_forks);
	is_anyone_dead(philo_parent); // here check for death
	sem_wait(philo_parent->sem_last_meal);
	philo_parent->shared_last_meal = get_time();
	sem_post(philo_parent->sem_last_meal);
	message(philo_parent, "is eating", philo.start_time);
	sem_post(philo_parent->sem_takes_forks);
	usleep(philo.tte * 1000);
	sem_post(philo_parent->sem_forks);
	sem_post(philo_parent->sem_forks);
}

// void	eating(t_philo_parent *philo_parent, t_philo_child philo)
// {
// 	if (philo.nb_philo % 2 == 0)
// 		usleep(20);
// 	philo_eats(philo_parent, philo);
// }

void	is_anyone_dead(t_philo_parent *philo_parent)
{
	sem_wait(philo_parent->sem_death_var);
	if (philo_parent->shared_died == 1)
	{
		// message(philo_parent, "here3", philo_parent->start_time);
		sem_post(philo_parent->sem_death_var);
		pthread_exit(NULL);
	}
	sem_post(philo_parent->sem_death_var);
}

void	*routine(void *void_philo)
{
	int		i;
	int		j;
	t_philo_child	philo;
	t_philo_parent	*philo_parent;

	i = 0;
	j = 0;
	philo_parent = copies_t_philo(&philo, void_philo);
	while (get_time() - philo.start_time < 1000)
		;
	if (philo.total == 1)
		pthread_exit(NULL);
	while (1)
	{
		philo_eats(philo_parent, philo);
		philo.last_meal = get_time() - philo.tte;
		is_anyone_dead(philo_parent);
		if (++i == philo.total_meals)
		{
			while (j++ < philo.total)
				sem_post(philo_parent->sem_meals);
		}
		message(philo_parent, "is sleeping", philo.start_time);
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			pthread_exit(NULL);
		is_anyone_dead(philo_parent);
		// message(philo_parent, "here2", philo.start_time);
		usleep(philo.tts * 1000);
		is_anyone_dead(philo_parent);
		message(philo_parent, "is thinking", philo.start_time);
	}
}

void	free_parent_thread(t_data *data, t_philo_parent *philo)
{
	free_data(data);
	close_semaphores(philo, NB_SEM);
	exit(1);
}

void	free_data(t_data *data)
{
	free(data->pid);
	sem_close(data->sem_parent_struct);
	sem_close(data->sem_init);
	sem_close(data->sem_death_var);
	sem_close(data->sem_meals);
	sem_close(data->sem_printf);
	sem_close(data->sem_forks);
	sem_close(data->sem_last_meal);
	sem_close(data->sem_takes_forks);
	sem_close(data->sem_death);
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
	int	nb_philo_done;
	int	total;

	nb_philo_done = 0;
	sem_wait(((t_philo_parent *)philo_void)->sem_parent_struct);
	philo_parent = (t_philo_parent *)philo_void;
	total = philo_parent->total;
	sem_post(philo_parent->sem_parent_struct);
	while (nb_philo_done != total)
	{
		is_anyone_dead(philo_parent);
		sem_wait(philo_parent->sem_meals);
		sem_wait(philo_parent->sem_nb_meals);
		nb_philo_done++;
		sem_post(philo_parent->sem_nb_meals);
		// sem_post(philo_parent->sem_meals);
		usleep(50);
	}
	sem_wait(philo_parent->sem_nb_meals);
	philo_parent->nb_meals = nb_philo_done;
	sem_post(philo_parent->sem_nb_meals);
	pthread_exit(NULL);
}

void    start(t_data *data, int nb_philo)
{
	t_philo_parent	philo_parent;
	size_t			last_meal;
	size_t			start_time;
	// int				tot;

	if (init_philo(data, &philo_parent, nb_philo) == 0)
		free_parent_thread(data, &philo_parent);
	philo_parent.shared_last_meal = philo_parent.start_time + (size_t)1000;
	pthread_create(&(philo_parent.philo_thread), NULL, routine, (void *)(&philo_parent)); // check for error
	pthread_create(&(philo_parent.death_thread), NULL, death_routine, (void *)(&philo_parent)); // check for error
	pthread_create(&(philo_parent.meals_thread), NULL, meals_routine, (void *)(&philo_parent));
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
		if ((int)last_meal >= philo_parent.ttd || philo_parent.shared_died == 1 || philo_parent.nb_meals > 0)
		{
			if (philo_parent.shared_died == 0)
			{
				if (philo_parent.nb_meals == 0)
					message(&philo_parent, "died", start_time);
				philo_parent.shared_died = 1;
				sem_post(philo_parent.sem_death);
			}
			sem_post(philo_parent.sem_nb_meals);
			sem_post(philo_parent.sem_death_var);
			pthread_join(philo_parent.death_thread, NULL);
			pthread_join(philo_parent.philo_thread, NULL);
			sem_post(philo_parent.sem_meals);
			pthread_join(philo_parent.meals_thread, NULL);
			free_parent_thread(data, &philo_parent);
		}
		sem_post(philo_parent.sem_nb_meals);
		sem_post(philo_parent.sem_death_var);
		usleep(50);
	}
}

// update void_philo si qqn d'autre meurt
// close TOUTES les sem a la fin
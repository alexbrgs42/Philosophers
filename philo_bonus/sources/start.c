/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:25:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/21 14:11:42 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

	// sem_t		*sem_died; // bin
	// sem_t		*sem_meals; // bin
	// sem_t		*sem_printf; // bin
	// sem_t		*sem_forks;

int	init_philo(t_data *data, t_philo_parent *philo, int nb_philo)
{
	sem_wait(data->sem_init);
	philo->sem_parent_struct = sem_open("/sem_parent_struct", O_CREAT | O_EXCL, 0644, 1);
	if (philo->sem_parent_struct == SEM_FAILED)
		return (0);
	philo->sem_died = sem_open("/sem_died", 0);
	if (philo->sem_died == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		return (0);
	}
	philo->sem_meals = sem_open("/sem_meals", 0);
	if (philo->sem_meals == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		return (0);
	}
	philo->sem_printf = sem_open("/sem_printf", 0);
	if (philo->sem_printf == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		sem_close(philo->sem_meals);
		return (0);
	}
	philo->sem_forks = sem_open("/sem_forks", 0);
	if (philo->sem_forks == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		sem_close(philo->sem_meals);
		sem_close(philo->sem_printf);
		return (0);
	}
	philo->sem_last_meal = sem_open("/sem_last_meal", 0);
	if (philo->sem_last_meal == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		sem_close(philo->sem_meals);
		sem_close(philo->sem_printf);
		sem_close(philo->sem_forks);
		return (0);
	}
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

int	copies_t_philo(t_philo_child *philo, void *void_philo)
{
	t_philo_parent	*philo_parent;

	sem_wait(((t_philo_parent *)void_philo)->sem_parent_struct);
	philo_parent = (t_philo_parent *)void_philo;
	philo->died = 0;
	philo->nb_philo = philo_parent->nb_philo;
	philo->tte = philo_parent->tte;
	philo->tts = philo_parent->tts;
	philo->ttd = philo_parent->ttd;
	philo->total = philo_parent->total;
	philo->start_time = philo_parent->start_time;
	sem_post(philo_parent->sem_parent_struct);
	return (philo_parent);
}

void	philo_eats(t_philo_parent *philo_parent, t_philo_child philo)
{
	sem_wait(philo_parent->sem_forks);
	message(philo_parent, philo, "has taken a fork");
	sem_wait(philo_parent->sem_forks);
	sem_wait(philo_parent->sem_last_meal);
	philo.last_meal = get_time();
	philo_parent->shared_last_meal = philo.last_meal;
	sem_post(philo_parent->sem_last_meal);
	message(philo_parent, philo, "is eating");
	usleep(philo.tte * 1000);
	sem_post(philo_parent->sem_forks);
	sem_post(philo_parent->sem_forks);
}

void	eating(t_philo_parent *philo_parent, t_philo_child philo)
{
	if (philo.nb_philo % 2 != 0)
		usleep(10);
	philo_eats(philo_parent, philo);
}

void	*routine(void *void_philo)
{
	int		i;
	t_philo_child	philo;
	t_philo_parent	*philo_parent;

	i = 0;
	philo_parent = copies_t_philo(&philo, void_philo);
	while (get_time() - philo.start_time < 1000)
		;
	single_philo(philo);
	while (1)
	{
		eating(philo_parent, philo);
		sem_wait(philo_parent->sem_meals);
		philo_parent->nb_meals++;
		sem_post(philo_parent->sem_meals);
		message(philo_parent, philo, "is sleeping"); //
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			finish(philo); //
		is_anyone_dead(philo_parent); //
		usleep(philo.tts * 1000);
		is_anyone_dead(philo_parent);
		message(philo_parent, philo, "is thinking");
	}
}

void    start(t_data *data, int nb_philo)
{
	t_philo_parent	philo_parent;
	int				last_meal;

	if (init_philo(data, &philo_parent, nb_philo) == 0)
		printf("help2\n"); // ...
	sem_close(philo_parent.sem_parent_struct);
	pthread_create(&(philo_parent.philo_thread), NULL, routine, (void *)(&philo_parent)); // check for error
	while (1)
	{
		sem_wait(philo_parent.sem_meals);
		if (philo_parent.total_meals == philo_parent.nb_meals)
			data->shared_philo_done++;
		sem_post(philo_parent.sem_meals);
		sem_wait(philo_parent.sem_last_meal);
		last_meal = philo_parent.shared_last_meal;
		sem_post(philo_parent.sem_last_meal);
		sem_wait(philo_parent.sem_died);
		if (last_meal >= philo_parent.ttd || data->shared_died == 1)
		{
			philo_parent.shared_died = data->shared_died + 1; // = 1 si lui meurt
			data->shared_died = 1;
			sem_post(philo_parent.sem_died);
			pthread_join(philo_parent.philo_thread, NULL);
			return (free_parent_thread(&philo_parent));
		}
		sem_post(philo_parent.sem_died);
		usleep(10);
	}
}

// update void_philo si qqn d'autre meurt (diff int si cest lui ou pas ?)
// close TOUTES les sem a la fin
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:25:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/22 17:43:17 by abourgeo         ###   ########.fr       */
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
	philo->sem_parent_struct = sem_open("/sem_parent_struct", 0);
	if (philo->sem_parent_struct == SEM_FAILED)
		return (printf("%s\n", strerror(errno)));
	philo->sem_died = sem_open("/sem_died", 0);
	if (philo->sem_died == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		return (printf("help5\n"));
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
	philo->sem_free_forks = sem_open("/sem_free_forks", 0);
	if (philo->sem_last_meal == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		sem_close(philo->sem_meals);
		sem_close(philo->sem_printf);
		sem_close(philo->sem_forks);
		sem_close(philo->sem_last_meal);
		return (0);
	}
	philo->shared_died = 0;
	philo->shared_free_forks = &(data->shared_free_forks);
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

t_philo_parent	*copies_t_philo(t_philo_child *philo, void *void_philo)
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

void	philo_odd(t_philo_parent *philo_parent, t_philo_child philo)
{
	int	state;

	state = 0;
	sem_wait(philo_parent->sem_forks);
	sem_post(philo_parent->sem_free_forks);
	message(philo_parent, philo, "has taken a fork");
	while (state < 1)
	{
		is_anyone_dead(philo_parent, philo);
		sem_wait(philo_parent->sem_free_forks);
		state = *(philo_parent->shared_free_forks);
		if (state >= 1)
			sem_wait(philo_parent->sem_forks);
		sem_post(philo_parent->sem_free_forks);
	}
	sem_wait(philo_parent->sem_last_meal);
	philo.last_meal = get_time();
	philo_parent->shared_last_meal = philo.last_meal;
	sem_post(philo_parent->sem_last_meal);
	message(philo_parent, philo, "is eating");
	usleep(philo.tte * 1000);
	sem_post(philo_parent->sem_forks);
	sem_post(philo_parent->sem_forks);
}

void	philo_eats(t_philo_parent *philo_parent, t_philo_child philo)
{
	sem_wait(philo_parent->sem_forks);
	message(philo_parent, philo, "has taken a fork");
	sem_wait(philo_parent->sem_forks);
	sem_post(philo_parent->sem_free_forks);
	sem_wait(philo_parent->sem_last_meal);
	philo_parent->shared_last_meal = get_time();
	sem_post(philo_parent->sem_last_meal);
	message(philo_parent, philo, "is eating");
	usleep(philo.tte * 1000);
	sem_post(philo_parent->sem_forks);
	sem_post(philo_parent->sem_forks);
}

void	eating(t_philo_parent *philo_parent, t_philo_child philo)
{
	int	state;

	state = 0;
	if (philo.nb_philo % 2 != 0)
		usleep(10);
	while (1) // state 1 other function call in which I sem_wait after he takes one fork
	{
		usleep(10);
		is_anyone_dead(philo_parent, philo);
		sem_wait(philo_parent->sem_free_forks);
		state = *(philo_parent->shared_free_forks);
		if (state >= 2)
			return (philo_eats(philo_parent, philo));
		if (state == 1)
			return (philo_odd(philo_parent, philo));
		sem_post(philo_parent->sem_free_forks);
	}
}

void	is_anyone_dead(t_philo_parent *philo_parent, t_philo_child philo)
{
	sem_wait(philo_parent->sem_died);
	if (philo_parent->shared_died == 2) // this philo died
		message(philo_parent, philo, "died");
	if (philo_parent->shared_died >= 1)
	{
		sem_post(philo_parent->sem_died);
		pthread_exit(NULL);
	}
	sem_post(philo_parent->sem_died);
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
	// single_philo(philo);
	while (1)
	{
		eating(philo_parent, philo);
		philo.last_meal = get_time() - philo.tte;
		sem_wait(philo_parent->sem_meals);
		philo_parent->nb_meals++;
		sem_post(philo_parent->sem_meals);
		message(philo_parent, philo, "is sleeping");
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			pthread_exit(NULL);
		is_anyone_dead(philo_parent, philo);
		usleep(philo.tts * 1000);
		is_anyone_dead(philo_parent, philo);
		message(philo_parent, philo, "is thinking");
	}
}

void	free_parent_thread(t_philo_parent *philo)
{
	sem_close(philo->sem_parent_struct);
	sem_close(philo->sem_died);
	sem_close(philo->sem_meals);
	sem_close(philo->sem_printf);
	sem_close(philo->sem_forks);
	sem_close(philo->sem_free_forks);
	sem_close(philo->sem_last_meal);
	pthread_join(philo->philo_thread, NULL);
	exit(-2);
}

void	free_data(t_data *data)
{
	free(data->pid);
	sem_close(data->sem_parent_struct);
	sem_close(data->sem_init);
	sem_close(data->sem_died);
	sem_close(data->sem_meals);
	sem_close(data->sem_printf);
	sem_close(data->sem_forks);
	sem_close(data->sem_free_forks);
	sem_close(data->sem_last_meal);
	// sem_unlink("/sem_died");
	// sem_unlink("/sem_init");
	// sem_unlink("/sem_meals");
	// sem_unlink("/sem_printf");
	// sem_unlink("/sem_forks");
	// sem_unlink("/sem_last_meal");
	// sem_unlink("/sem_free_forks");
	// sem_unlink("/sem_parent_struct");
}

void    start(t_data *data, int nb_philo)
{
	t_philo_parent	philo_parent;
	size_t			last_meal;

	if (init_philo(data, &philo_parent, nb_philo) == 0)
		printf("help2\n"); // ...
	philo_parent.shared_last_meal = philo_parent.start_time + (size_t)1000;
	pthread_create(&(philo_parent.philo_thread), NULL, routine, (void *)(&philo_parent)); // check for error
	pthread_detach(philo_parent.philo_thread);
	free_data(data);
	while (get_time() - philo_parent.start_time < 1000)
		;
	while (1)
	{
		sem_wait(philo_parent.sem_meals);
		if (philo_parent.total_meals == philo_parent.nb_meals)
			data->shared_philo_done++;
		sem_post(philo_parent.sem_meals);
		sem_wait(philo_parent.sem_last_meal);
		last_meal = get_time() - philo_parent.shared_last_meal;
		sem_post(philo_parent.sem_last_meal);
		sem_wait(philo_parent.sem_died);
		if (last_meal >= (size_t)philo_parent.ttd || data->shared_died == 1)
		{
			// philo_parent.shared_died = (last_meal >= (size_t)philo_parent.ttd) + 1; // = 2 si lui meurt
			// data->shared_died = 1;
			sem_post(philo_parent.sem_died);
			free_parent_thread(&philo_parent);
		}
		sem_post(philo_parent.sem_died);
		usleep(10);
	}
}

// update void_philo si qqn d'autre meurt (diff int si cest lui ou pas ?)
// close TOUTES les sem a la fin
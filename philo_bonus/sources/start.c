/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:25:34 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/19 19:14:56 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

	// sem_t		*sem_died; // bin
	// sem_t		*sem_meals; // bin
	// sem_t		*sem_printf; // bin
	// sem_t		*sem_forks;
	// sem_t		*sem_free_forks; // bin

int	init_philo(t_data *data, t_philo *philo, int nb_philo)
{
	sem_wait(data->sem_init);
	philo->sem_parent_struct = sem_open("/sem_philo", O_CREAT | O_EXCL, 0644, 1);
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
	philo->sem_free_forks = sem_open("/sem_free_forks", 0);
	if (philo->sem_free_forks == SEM_FAILED)
	{
		sem_close(philo->sem_parent_struct);
		sem_close(philo->sem_died);
		sem_close(philo->sem_meals);
		sem_close(philo->sem_printf);
		sem_close(philo->sem_forks);
		return (0);
	}
	philo->died = 0;
	philo->nb_philo = nb_philo;
	philo->tte = data->time_to_eat;
	philo->tts = data->time_to_sleep;
	philo->ttd = data->time_to_die;
	philo->total = data->number_of_philosophers;
	philo->start_time = data->start_time;
	sem_post(data->sem_init);
	return (1);
}

int	copies_t_philo(t_philo *philo, void *void_philo)
{
	sem_wait(((t_philo *)void_philo)->sem_parent_struct);
	philo->sem_parent_struct = sem_open("/sem_philo", 0);
	if (philo->sem_parent_struct == SEM_FAILED)
		return (0);
	philo->died = 0;
	philo->nb_philo = ((t_philo *)void_philo)->nb_philo;
	philo->tte = ((t_philo *)void_philo)->tte;
	philo->tts = ((t_philo *)void_philo)->tts;
	philo->ttd = ((t_philo *)void_philo)->ttd;
	philo->total = ((t_philo *)void_philo)->total;
	philo->start_time = ((t_philo *)void_philo)->start_time;
	sem_post(philo->sem_parent_struct);
	return (1);
}

void	philo_pair(t_philo philo)
{
	
}

void	eating(t_philo philo)
{
	if (philo.nb_philo % 2 == 0)
		philo_pair(philo);
	else
	{
		usleep(10);
		philo_impair(philo);
	}
}

void	*routine(void *void_philo)
{
	int		i;
	t_philo	philo;

	i = 0;
	if (copies_t_philo(&philo, void_philo) == 0)
		return ; // ....
	while (get_time() - philo.start_time < 1000)
		;
	single_philo(philo);
	while (1)
	{
		eating(philo);
		increment_number_meals(philo, &i);
		philo.last_meal = get_time() - philo.tte;
		message(philo, "is sleeping");
		if ((int)(get_time() - philo.last_meal + philo.tts) > philo.ttd)
			about_to_die(philo);
		is_anyone_dead((t_philo *)void_philo);
		usleep(philo.tts * 1000);
		is_anyone_dead((t_philo *)void_philo);
		message(philo, "is thinking");
	}
}

void    start(t_data *data, int nb_philo)
{
	t_philo	philo;

	if (init_philo(data, &philo, nb_philo) == 0)
		; // ...
	pthread_create(&(philo.philo_thread), NULL, routine, (void *)(&philo)); // check for error
	// check for death
	// update void_philo si qqn d'autre meurt (diff int si cest lui ou pas ?)
	// close les sem PARTOUT a la fin
}

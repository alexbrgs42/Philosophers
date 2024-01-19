/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:55:04 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/19 19:09:37 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// SEM_VALUE_MAX


#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# ifndef NB_SEM
#  define NB_SEM 6
# endif

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <sys/types.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <semaphore.h>

typedef struct s_data
{
	int		*pid;
	int		died;
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	int		philo_done; // check nb_meals
	size_t	start_time;
	sem_t	*sem_init; // bin
	sem_t	*sem_died; // bin
	sem_t	*sem_meals; // bin
	sem_t	*sem_printf; // bin
	sem_t	*sem_forks;
	sem_t	*sem_free_forks; // bin
}	t_data;

typedef struct s_philo
{
	int			nb_philo;
	int			died;
	int			tte;
	int			tts;
	int			ttd;
	int			total;
	size_t		last_meal;
	size_t		start_time;
	pthread_t	philo_thread;
	sem_t		*sem_parent_struct;
	sem_t		*sem_died; // bin
	sem_t		*sem_meals; // bin
	sem_t		*sem_printf; // bin
	sem_t		*sem_forks;
	sem_t		*sem_free_forks; // bin
}	t_philo;

// initialization.c

int	 	init_data(t_data *data, char *argv[]);
int 	init_semaphores(t_data *data);
int 	init_numbers(t_data *data, char *argv[]);

// freeing.c

int		free_semaphores(t_data *data, int i);
void	free_semaphores_bis(t_data *data, int i);

// start.c

void    init_philo(t_data *data, t_philo *philo, int nb_philo);
void    start(t_data *data, int nb_philo);

// utils.c

void	message(t_philo philo, char *str);
int		ft_strlen(char *str);
size_t	get_time(void);
int		ft_atoi(char *str, int *num);

// main.c

#endif
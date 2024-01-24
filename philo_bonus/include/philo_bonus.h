/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:55:04 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 20:45:21 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# ifndef NB_SEM
#  define NB_SEM 11
# endif

# ifndef MACROS
#  define SEM_DEATH_VAR "/sem_death_var"
#  define SEM_INIT "/sem_init"
#  define SEM_MEALS "/sem_meals"
#  define SEM_PRINTF "/sem_printf"
#  define SEM_FORKS "/sem_forks"
#  define SEM_LAST_MEAL "/sem_last_meal"
#  define SEM_PARENT_STRUCT "/sem_parent_struct"
#  define SEM_DEATH "/sem_death"
#  define SEM_TAKES_FORKS "/sem_takes_forks"
#  define SEM_NB_MEALS "/sem_nb_meals"
#  define SEM_INCREMENT "/sem_increment"
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
# include <signal.h>
# include <errno.h>

typedef struct s_data
{
	int			*pid;
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_times_each_philosopher_must_eat;
	pthread_t	thread_meals;
	size_t		start_time;
	sem_t		*sem_init;
	sem_t		*sem_death_var;
	sem_t		*sem_meals;
	sem_t		*sem_printf;
	sem_t		*sem_forks;
	sem_t		*sem_last_meal;
	sem_t		*sem_parent_struct;
	sem_t		*sem_death;
	sem_t		*sem_takes_forks;
	sem_t		*sem_nb_meals;
	sem_t		*sem_increment;
}	t_data;

typedef struct s_philo_parent
{
	int			nb_philo;
	int			shared_died;
	int			tte;
	int			tts;
	int			ttd;
	int			nb_meals;
	int			total_meals;
	int			total;
	size_t		shared_last_meal;
	size_t		start_time;
	pthread_t	philo_thread;
	pthread_t	death_thread;
	pthread_t	meals_thread;
	sem_t		*sem_parent_struct;
	sem_t		*sem_death_var;
	sem_t		*sem_meals;
	sem_t		*sem_printf;
	sem_t		*sem_forks;
	sem_t		*sem_last_meal;
	sem_t		*sem_death;
	sem_t		*sem_takes_forks;
	sem_t		*sem_nb_meals;
	sem_t		*sem_increment;
}	t_philo_parent;

typedef struct s_philo_child
{
	int			nb_philo;
	int			total_meals;
	int			tte;
	int			tts;
	int			ttd;
	int			total;
	size_t		last_meal;
	size_t		start_time;
}	t_philo_child;

// free_philo.c

void			free_parent_thread(t_data *data, t_philo_parent *philo);
void			free_data(t_data *data);
int				close_semaphores(t_philo_parent *philo, int i);

// init_philo.c

int				init_var_philo(t_data *data, t_philo_parent *philo,
					int nb_philo);
int				init_philo(t_data *data, t_philo_parent *philo, int nb_philo);
t_philo_parent	*copies_t_philo(t_philo_child *philo, void *void_philo);

// initialization.c

int				init_data(t_data *data, char *argv[]);
int				nb_digits(int i);
int				init_semaphores(t_data *data);
int				init_semaphores_bis(t_data *data);
int				init_numbers(t_data *data, char *argv[]);

// freeing.c

void			final_free(t_data *data);
int				free_semaphores(t_data *data, int i);
void			free_semaphores_bis(t_data *data, int i);
void			free_semaphores_bis_bis(t_data *data, int i);

// thread_routine.c

void			*routine(void *void_philo);
void			philo_has_eaten_enough(t_philo_parent *philo_parent,
					int total);
void			*death_routine(void *philo);
void			*meals_routine(void *philo_void);

// utils_philo.c

void			philo_eats(t_philo_parent *philo_parent, t_philo_child philo);
void			is_anyone_dead(t_philo_parent *philo_parent, int nb_forks,
					int nb);

// start.c

void			start(t_data *data, int nb_philo);
void			start_routines(t_data *data, t_philo_parent *philo_parent,
					int nb_philo);
void			end_routines(t_data *data, t_philo_parent *philo_parent,
					int nb_philo, size_t start_time);

// utils.c

void			message(t_philo_parent *philo_parent, char *str,
					size_t start_time);
int				ft_strlen(char *str);
size_t			get_time(void);
int				ft_atoi(char *str, int *num);

// main.c

void			set_semaphore_state(void);

#endif
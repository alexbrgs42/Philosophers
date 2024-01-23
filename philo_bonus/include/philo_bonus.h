/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:55:04 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/23 11:18:07 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// SEM_VALUE_MAX


#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# ifndef NB_SEM
#  define NB_SEM 9
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

#  define DEATH 1
#  define MAX_MEALS 2

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
	int		*pid;
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	size_t	start_time;
	sem_t	*sem_init; // bin
	sem_t	*sem_death_var; // bin
	sem_t	*sem_meals; // bin
	sem_t	*sem_printf; // bin
	sem_t	*sem_forks;
	sem_t	*sem_last_meal; // bin
	sem_t	*sem_parent_struct;
	sem_t	**sem_death; // bin for each sem_death[i]
	sem_t	*sem_takes_forks;
}	t_data;
// make sem_t tabs for each philo process if not dealing with t_data data structure

typedef struct s_philo_parent
{
	int			nb_philo;
	int			shared_died; // shared
	int			tte;
	int			tts;
	int			ttd;
	int			nb_meals;
	int			total_meals;
	int			total;
	size_t		shared_last_meal; // shared
	size_t		start_time;
	pthread_t	philo_thread;
	pthread_t	death_thread;
	pthread_t	meals_thread;
	sem_t		*sem_parent_struct; // closed at beginning by each child process
	sem_t		*sem_death_var; // bin - for parent thread and main process
	sem_t		*sem_meals; // bin
	sem_t		*sem_printf; // bin
	sem_t		*sem_forks;
	sem_t		*sem_last_meal; // bin
	sem_t		*sem_death; // data->sem_death[nb_philo]
	sem_t		*sem_takes_forks;
}	t_philo_parent;

typedef struct s_philo_child
{
	int			nb_philo;
	int			died; // update shared_died if modif
	int			tte;
	int			tts;
	int			ttd;
	int			total;
	size_t		last_meal; // update shared_last_meal if modif
	size_t		start_time;
}	t_philo_child;


// initialization.c

int		init_data(t_data *data, char *argv[]);
int		init_semaphores(t_data *data);
int		init_numbers(t_data *data, char *argv[]);
void	ft_putnbr_tab(char *res, int i, int *j);
char	*ft_strjoin(char *start, int i);
int		close_sem_death(t_data *data, int i, int unlinking);
int		init_sem_death_tab(t_data *data);
int		nb_digits(int i);

// freeing.c

void    final_free(t_data *data);
int		free_semaphores(t_data *data, int i);
void	free_semaphores_bis(t_data *data, int i);

// start.c

int		init_philo(t_data *data, t_philo_parent *philo, int nb_philo);
t_philo_parent	*copies_t_philo(t_philo_child *philo, void *void_philo);
void	philo_odd(t_philo_parent *philo_parent, t_philo_child philo);
void	philo_eats(t_philo_parent *philo_parent, t_philo_child philo);
void	eating(t_philo_parent *philo_parent, t_philo_child philo);
void	is_anyone_dead(t_philo_parent *philo_parent, t_philo_child philo);
void	*routine(void *void_philo);
void	start(t_data *data, int nb_philo);
int		close_semaphores(t_philo_parent *philo, int i);

// utils.c

void	message(t_philo_parent *philo_parent, char *str);
int		ft_strlen(char *str);
size_t	get_time(void);
int		ft_atoi(char *str, int *num);

// main.c

#endif
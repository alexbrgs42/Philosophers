/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:53:40 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/24 20:25:19 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data
{
	int				*forks;
	pthread_t		*philo;
	int				died;
	int				bool_printf;
	int				counter;
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				philo_done;
	size_t			start_time;
	pthread_mutex_t	mutex_struct;
	pthread_mutex_t	mutex_died;
	pthread_mutex_t	mutex_meals;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	*mutex_forks_var;
	pthread_mutex_t	mutex_begin;
}	t_data;

typedef struct s_philo
{
	int		nb_philo;
	int		tte;
	int		tts;
	int		ttd;
	int		total;
	size_t	last_meal;
	size_t	start_time;
}	t_philo;

// eating.c

void	take_fork(t_data *data, t_philo philo, int first_fork, int second_fork);
void	eating(t_data *data, t_philo philo);
int		check_loop(t_data *data, int first_fork, int second_fork);
int		count_free_forks(t_data *data, int first_fork, int second_fork);

// finish.c

void	single_philo(t_data *data, t_philo philo);
void	check_for_dead(t_data *data, int first_fork, int second_fork);
void	philo_died(t_data *data, t_philo philo);
void	philo_finished(t_data *data);
void	about_to_die(t_data *data, t_philo philo);

// freeing.c

int		first_free(t_data *data, int nb);
int		final_free(t_data *data, int nb_thread, int join, int return_val);

//initialization.c

int		init_data(t_data *data, char *argv[]);
int		init_numbers(t_data *data, char *argv[]);
int		init_forks(t_data *data);
void	init_t_philo(t_data *data, t_philo *philo);

// init_mutex.c

int		init_mutex_forks_var(t_data *data);
int		init_mutex_forks(t_data *data);

// utils.c

void	message(t_data *data, t_philo philo, char *str);
void	ft_usleep(t_data *data, size_t t, int first_fork, int second_fork);
int		ft_strlen(char *str);
size_t	get_time(void);
int		ft_atoi(char *str, int *number);

// count_meals.c

void	increment_number_meals(t_data *data, t_philo philo, int *i);

// main.c

int		check_loop(t_data *data, int first_fork, int second_fork);
int		ft_strcmp(char *s1, char *s2);
void	*routine(void *data_void);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:53:40 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/17 22:02:08 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data
{
	int				*forks;
	pthread_t		*philo;
	int				died;
	int				counter; // numeroter philos
	int				number_of_philosophers; // number_of_philosophers
	int				time_to_die; // time_to_die
	int				time_to_eat; // time_to_eat
	int				time_to_sleep; // time_to_sleep
	int				number_of_times_each_philosopher_must_eat; // number_of_times_each_philosopher_must_eat
	size_t			start_time;
	pthread_mutex_t	mutex_struct;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_forks_var;
}	t_data;

// freeing.c

int		first_free(t_data *data, int pr, int str, int fo);
int		final_free(t_data *data, int nb_thread, int join, int return_val);

//initialization.c

int		init_data(t_data *data, char *argv[]);
int		init_numbers(t_data *data, char *argv[]);
int		init_mutex_forks(t_data *data);
int		init_forks(t_data *data);

// utils.c

int		ft_strlen(char *str);
size_t	get_time(void);
int		ft_atoi(char *str, int *number);

// main.c

#endif
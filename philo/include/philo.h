/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 13:53:40 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/17 14:18:30 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>

typedef struct s_data
{
	int				*forks;
	int				*pid;
	pthread_t		*philo;
	int				nb_philo; // number_of_philosophers
	int				ttd; // time_to_die
	int				tte; // time_to_eat
	int				tts; // time_to_sleep
	int				nb_meals; // number_of_times_each_philosopher_must_eat
	size_t			start_time;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	*mutex_forks;
	pthread_mutex_t	mutex_args; // for ttd, tte, tts, nb_meals, start_time
}	t_data;

// main.c

#endif
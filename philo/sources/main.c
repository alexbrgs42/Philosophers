/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourgeo <abourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 10:42:01 by abourgeo          #+#    #+#             */
/*   Updated: 2024/01/17 13:26:41 by abourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

size_t	get_time(void);

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
int			x = 0;

void	*routine(void *start_time)
{
	for (int i = 0; i < 1000000; i++)
	{
		pthread_mutex_lock(&mutex1);
		x++;
		pthread_mutex_unlock(&mutex1);
	}
	printf("Current time : %zu\n", get_time() - *((size_t *) start_time));
	return (NULL);
}

size_t	get_time(void)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	main(void)
{
	pthread_t	t1[4];
	int			i;
	size_t		start_time;
	size_t		current_time;

	i = 0;
	start_time = get_time();
	printf("Start time : %zu\n", start_time);
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	while (i < 4)
	{
		pthread_create(&t1[i], NULL, &routine, (void *) (&start_time));
		printf("Thread number %d started\n", i);
		i++;
	}
	current_time = get_time() - start_time;
	i = 0;
	while (i < 4)
	{
		pthread_join(t1[i], NULL);
		printf("Thread number %d has finished\n", i);
		i++;
	}
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	printf("x = %d\n", x);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:36:06 by prizmo            #+#    #+#             */
/*   Updated: 2024/07/01 13:02:56 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_philo	*philo)
{
	//
}

size_t	get_time()
{
	size_t			curr_time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	curr_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (curr_time / 10);
}

void	*routine()
{
	printf("Hello\n");
	return (0);
}

int	main(int ac, char **av)
{
	pthread_mutex_t	mutex;
	pthread_t		thread_1;
	pthread_t		thread_2;

	if (pthread_create(&thread_1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&thread_2, NULL, &routine, NULL) != 0)
		return (1);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	printf("%ld\n", get_time());
	return (0);
}
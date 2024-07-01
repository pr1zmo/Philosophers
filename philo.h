/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:51:53 by prizmo            #+#    #+#             */
/*   Updated: 2024/07/01 12:48:48 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t	thread;
	int			number;
	int			is_eating;
	int			is_thinking;
	int			is_sleeping;
}	t_philo;

void	init_philos(t_philo	*philo);

#endif
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
# include <limits.h>
# include <sys/time.h>

# define ALLOC_ERR_1 "ERROR WHILE ALLOCATING THREADS IDs"
# define ALLOC_ERR_3 "ERROR WHILE ALLOCATING PHILOS"
# define ALLOC_ERR_2 "ERROR WHILE ALLOCATING FORKS"
# define ERR_IN_1 "INVALID INPUT CHARACTER"
# define ERR_IN_2 "INVALID INPUT VALUES"
# define ERR_IN_3 "INVALID NUMBER OF PARAMETERS"
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"
# define TIME_ERR "UNABLE TO RETRIVE UTC"
# define TAKE_FORKS "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "died"

typedef	struct s_data
{
	int	philos_number;
	int	time_to_eat;
	int	time_to_die;
	int	time_to_sleep;
}	t_data;

typedef struct s_philo
{
	pthread_t	    thread;
	int			    id;
	int			    is_eating;
	int			    is_thinking;
	int			    is_sleeping;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
}	t_philo;

#endif
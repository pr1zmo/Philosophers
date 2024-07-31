/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 16:42:57 by prizmo            #+#    #+#             */
/*   Updated: 2024/07/31 14:23:17 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

// Colors
# define RESET		"\033[0m"
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

// Background Colors
# define BG_BLACK	"\033[40m"
# define BG_RED		"\033[41m"
# define BG_GREEN	"\033[42m"
# define BG_YELLOW	"\033[43m"
# define BG_BLUE	"\033[44m"
# define BG_MAGENTA	"\033[45m"
# define BG_CYAN	"\033[46m"
# define BG_WHITE	"\033[47m"

// Styles
# define BOLD		"\033[1m"
# define UNDERLINE	"\033[4m"
# define REVERSED	"\033[7m"

typedef struct s_data
{
	int				philo_count;
	int				eat_time;
	int				sleep_time;
	int				death_time;
	int				must_eat_count;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	death_lock;
	int				simulation_end;
}	t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				eating;
	int				is_dead;
	int				nbr_of_meals;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	m_lock;
	t_data			*data;
	size_t			last_meal_time;
	size_t			last_action;
}	t_philo;

int	alive(t_philo *philo);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 16:42:57 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/02 14:24:27 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

# define RESET		"\033[0m"
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

# define BG_BLACK	"\033[40m"
# define BG_RED		"\033[41m"
# define BG_GREEN	"\033[42m"
# define BG_YELLOW	"\033[43m"
# define BG_BLUE	"\033[44m"
# define BG_MAGENTA	"\033[45m"
# define BG_CYAN	"\033[46m"
# define BG_WHITE	"\033[47m"

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

int		check_params(char **av, int const ac);
void	init_data(char **av, int ac, t_data *data);
void	init_forks(pthread_mutex_t *forks, char **av);
void	init_philos(pthread_mutex_t *fork, t_data *data, t_philo *philo);
int		check_data(t_data *data, t_philo *philo);
void	start_simulation(t_data *data, pthread_mutex_t *forks,
		t_philo *philos);
void	destroy_all(char *str, t_philo *philo, pthread_mutex_t *forks);
int		all_ate(t_philo *philo);
int		starved(t_philo *philo);
int		alive(t_philo *philo);
void	eat(t_philo *philo);
void	think(t_philo *philo);
void	rest(t_philo *philo);
void	write_message(t_philo *philo, char *str);
void	*monitor(void *param);
void	*routine(void *data);
int		ft_strlen(char *str);
int		ft_isdigit(char	const *c);
int		ft_atoi(const char *str);
int		ft_usleep(size_t milliseconds);
int		check_death(t_philo philo);
size_t	get_time(void);

#endif
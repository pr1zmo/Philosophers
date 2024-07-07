/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:37:34 by zelbassa          #+#    #+#             */
/*   Updated: 2024/07/07 19:18:47 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char	const *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (c[i] >= '0' && c[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					sign;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = (-1 * (*str == '-')) + (*str == '+');
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result > (unsigned long long)LLONG_MAX && sign == 1)
			return (-1);
		else if (result > (unsigned long long)LLONG_MAX && sign == -1)
			return (0);
		str++;
	}
	return ((int)(result * sign));
}

int	check_params(char **av, int const ac)
{
	int	i;

	i = 0;
	while (i < ac - 1)
	{
		if (ft_isdigit(av[i + 1]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	*routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	printf("Hello\n");
	pthread_join(philo[0].thread, NULL);
	return (0);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	init_data(char **av, int ac, t_data *data, t_philo philos)
{
	int	i;

	i = 0;
	data->philo_count = ft_atoi(av[1]);
	if (data->philo_count > 200)
	{
		printf("%s\n", ERR_IN_2);
		exit(1);
	}
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->eating_rounds = ft_atoi(av[5]);
	pthread_mutex_init(&data->stdout_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	data->philos = &philos;
}

void	init_forks(pthread_mutex_t *forks, char **av)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_philos(pthread_mutex_t *forks, t_data *data, t_philo *philos)
{
	int		i;
	i = 0;
	while (i < data->philo_count)
	{
		philos[i].id = i + 1;
		philos[i].is_eating = 0;
		philos[i].last_meal = get_current_time();
		philos[i].lfork = &forks[i];
		philos[i].death_lock = &data->death_lock;
		philos[i].stdout_lock = &data->stdout_lock;
		if (i == 0)
			philos[i].rfork = &forks[data->philo_count - 1];
		else
			philos[i].rfork = &forks[i - 1];
		i ++;
	}
}

void	start_simulation(t_data *data, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	printf("%d\n", data->philo_count);
	while (i < data->philo_count)
	{
		pthread_create(&data->philos[i].thread, NULL, &routine, &data->philos[i]);
		printf("philo id: %d\n", data->philos[i].id);
		i++;
	}
}

void	destroy_all(t_philo *philos, t_data *data, pthread_mutex_t *fords)
{
	//
}

int	main(int ac, char **av)
{
	t_data			data;
	t_philo			philos[200];
	pthread_mutex_t	forks[200];

	if (ac != 5 && ac != 6)
		return (printf("%s\n", ERR_IN_3), 1);
	if (!check_params(av, ac))
		return (printf("%s\n", ERR_IN_2), 1);
	init_data(av, ac, &data, *philos);
	init_forks(forks, av);
	init_philos(forks, &data, philos);
	start_simulation(&data, forks);
	destroy_all(philos, &data, forks);
	return (0);
}
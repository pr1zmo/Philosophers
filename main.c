/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:01:02 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/07 12:38:39 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

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
	int	res;
	int	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t'
			|| *str == '\v' || *str == '\f' || *str == '\r'))
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		++str;
	}
	return (res * negative);
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

int	check_data(t_data *data)
{
	if (data->philo_count < 1 || data->philo_count > 200)
		return (printf("Invalid number of philosophers\n"), 0);
	if (data->eat_time < 60 || data->sleep_time < 60 || data->death_time < 60)
		return (printf("Invalid time values\n"), 0);
	if (data->meal_count < -1)
		return (printf("Invalid number of meals\n"), 0);
	return (1);
}

u_int64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int	init_data(char **av, int ac, t_data *data)
{
	data->philo_count = ft_atoi(av[1]);
	data->death_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->meal_count = -1;
	if (ac == 6)
		data->meal_count = ft_atoi(av[5]);
	data->finish = 0;
	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	return(check_data(data));
}

void	init_forks(pthread_mutex_t *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void	init_philos(t_philo *philo, pthread_mutex_t *forks, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].id = i + 1;
		philo[i].eating = 0;
		philo[i].is_dead = 0;
		philo[i].nbr_of_meals = 0;
		philo[i].last_meal_time = get_time();
		if (i % 2 != 0)
		{
			philo[i].rfork = &forks[i];
			philo[i].lfork = &forks[(i + 1) % data->philo_count];
		}
		else
		{
			philo[i].lfork = &forks[i];
			philo[i].rfork = &forks[(i + 1) % data->philo_count];
		}
		philo[i].data = data;
		pthread_mutex_init(&philo[i].m_lock, NULL);
		i++;
	}
}

void	*monitor(void *data)
{
	t_philo	*philos;

	philos = (t_philo *)data;
}

void	start_simulation(t_philo *philos)
{
	pthread_t	monitor_t;
	int			i;

	i = 0;
	pthread_create(monitor_t, NULL, monitor, philos);
}

int	main(int ac, char **av)
{
	t_philo			philos[200];
	t_data			data;
	pthread_mutex_t	forks[200];

	if (ac < 5 || ac > 6)
		return (printf("Wrong number of argumenst\n"), 1);
	if (!check_params(av, ac))
		return (printf("Wrong arguments values\n"), 1);
	if (!init_data(av, ac, &data))
		return (1);
	init_forks(forks, data.philo_count);
	init_philos(philos, forks, &data);
	start_simulation(philos);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:37:34 by zelbassa          #+#    #+#             */
/*   Updated: 2024/07/31 19:14:43 by prizmo           ###   ########.fr       */
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
	int	res;
	int	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
			*str == '\v' || *str == '\f' || *str == '\r'))
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

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	init_data(char **av, int ac, t_data *data)
{
	data->philo_count = ft_atoi(av[1]);
	data->death_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat_count = ft_atoi(av[5]);
	else
		data->must_eat_count = -1;
	data->simulation_end = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
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

void	init_philos(pthread_mutex_t *fork, t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].id = i + 1;
		philo[i].eating = 0;
		philo[i].is_dead = 0;
		philo[i].nbr_of_meals = 0;
		philo[i].lfork = &fork[i];
		philo[i].rfork = &fork[(i + 1) % data->philo_count];
		philo[i].data = data;
		pthread_mutex_init(&philo[i].m_lock, NULL);
		philo[i].last_meal_time = get_time();
		philo[i].last_action = get_time();
		i++;
	}
}

int	check_data(t_data *data, t_philo *philo)
{
	if (data->philo_count < 1 || data->philo_count > 200)
	{
		printf("%d\n", data->philo_count);
		return (printf("Invalid number of philosophers\n"), 0);
	}
	if (data->eat_time < 60 || data->sleep_time < 60 || data->death_time < 60)
		return (printf("Invalid time values\n"), 0);
	if (data->must_eat_count < -1)
		return (printf("Invalid number of meals\n"), 0);
	return (1);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int	all_ate(t_philo *philo)
{
	int i;
	int	k;

	k = 0;
	i = 0;
	while (i < philo->data->philo_count)
	{
		pthread_mutex_lock(&philo->m_lock);
		if (philo->data->must_eat_count == philo[i].nbr_of_meals)
			k++;
		i++;
		pthread_mutex_unlock(&philo->m_lock);
	}
	if (k == philo->data->philo_count)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		philo->data->simulation_end = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		return (1);
	}
	return (0);
}

int	check_death(t_philo philo)
{
	pthread_mutex_lock(&philo.data->meal_lock);
	if (get_time() - philo.last_meal_time >= philo.data->death_time
		&& philo.eating == 0)
	{
		pthread_mutex_unlock(&philo.data->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo.data->meal_lock);
	return (0);
}

void	write_message(t_philo *philo, char *str)
{
	size_t	timestamp;

	timestamp = get_time() - philo->last_action;
	pthread_mutex_lock(&philo->data->print_lock);
	if (alive(philo))
		printf("%d %d %s\n", timestamp, philo->id, str);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	starved(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->philo_count)
	{
		if (check_death(philo[i]))
		{
			write_message(&philo[i], "has died");
			pthread_mutex_lock(&philo->data->death_lock);
			philo->data->simulation_end = 1;
			pthread_mutex_unlock(&philo->data->death_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	while (1)
	{
		if (all_ate(philo) || starved(philo))
			break ;
	}
	return (param);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->rfork);
	write_message(philo, "Has taken a fork");
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->death_time);
		pthread_mutex_unlock(philo->rfork);
		return ;
	}
	pthread_mutex_lock(philo->lfork);
	write_message(philo, "has taken a fork");
	write_message(philo, "is eating");

	pthread_mutex_lock(&philo->data->meal_lock);
	philo->eating = 1;
	philo->nbr_of_meals += 1;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->meal_lock);

	ft_usleep(philo->data->eat_time);

	pthread_mutex_lock(&philo->data->meal_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->data->meal_lock);

	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(philo->lfork);
}

void	think(t_philo *philo)
{
	write_message(philo, "is thinking");
}

void	rest(t_philo *philo)
{
	write_message(philo, "is sleeping");
	ft_usleep(philo->data->sleep_time);
}

int	alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->simulation_end)
		return (pthread_mutex_unlock(&philo->data->death_lock), 0);
	return (pthread_mutex_unlock(&philo->data->death_lock), 1);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		usleep(100);
	while (alive(philo))
	{
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (data);
}

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	destroy_all(char *str, t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&philo->data->print_lock);
	pthread_mutex_destroy(&philo->data->meal_lock);
	pthread_mutex_destroy(&philo->data->death_lock);
	while (i < philo->data->philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	start_simulation(t_data *data, pthread_mutex_t *forks, t_philo *philos)
{
	pthread_t	m_thread;
	int			i;

	i = 0;
	if (pthread_create(&m_thread, NULL, &monitor, philos))
		return ;
	while (i < data->philo_count)
	{
		pthread_create(&philos[i].thread, NULL, &routine, &philos[i]);
		i++;
	}
	i = 0;
	pthread_join(m_thread, NULL);
	while (i < data->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return ;
}

int	main(int ac, char **av)
{
	t_data			data;
	t_philo			philos[200];
	pthread_mutex_t	forks[200];

	if (ac != 5 && ac != 6)
		return (printf("Invalid number of arguments\n"), 1);
	if (!check_params(av, ac))
		return (printf("Invalid input values\n"), 1);
	init_data(av, ac, &data);
	init_forks(forks, av);
	init_philos(forks, &data, philos);
	if (!check_data(&data, philos))
		return (1);
	start_simulation(&data, forks, philos);
	destroy_all(NULL, philos, forks);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:01:02 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/08 10:33:35 by prizmo           ###   ########.fr       */
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
		philo[i].first_action = get_time();
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

int	all_ate(t_philo *philos)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (i < philos->data->philo_count)
	{
		pthread_mutex_lock(&philos[i].m_lock);
		if (philos[i].nbr_of_meals >= philos->data->meal_count)
			k++;
		i++;
		pthread_mutex_unlock(&philos[i].m_lock);
	}
	if (k == philos->data->philo_count)
	{
		pthread_mutex_lock(&philos->data->death_lock);
		philos->data->finish = 1;
		pthread_mutex_unlock(&philos->data->death_lock);
		return (1);
	}
	return (0);
}

int	starved(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->philo_count)
	{
		pthread_mutex_lock(&philos->data->death_lock);
		if (get_time() - philos[i].last_meal_time > philos->data->death_time)
		{
			philos->data->finish = 1;
			return (pthread_mutex_unlock(&philos->data->death_lock), 1);
		}
		i++;
	}
	return (0);
}

int	alive(t_philo *philo)
{
	if (philo->data->meal_count != -1 && philo->nbr_of_meals >= philo->data->meal_count)
		return (0);
	if (get_time() - philo->last_meal_time > philo->data->death_time)
	{
		pthread_mutex_lock(&philo->m_lock);
		write_message(philo, philo->id, DEAD);
		pthread_mutex_unlock(&philo->m_lock);
		return (0);
	}
	return (1);
}

void	write_message(t_philo *philo, int id, char *str)
{
	u_int64_t curr_time;

	curr_time = get_time() - philo->first_action;
	pthread_mutex_lock(&philo->data->print_lock);
	if (alive(philo))
		printf("%lu %d %s\n", curr_time, id, str);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	*monitor(void *data)
{
	t_philo	*philos;

	philos = (t_philo *)data;
	while (1)
	{
		if ((philos->data->meal_count != -1) && all_ate(philos))
			break;
		if (starved(philos))
			break;
		ft_usleep(50);
	}
	return (philos);
}

void	take_forks(pthread_mutex_t *fork_1, pthread_mutex_t *fork_2, t_philo *philo)
{
	pthread_mutex_lock(fork_1);
	write_message(philo, philo->id, FORK);
	pthread_mutex_lock(fork_2);
	write_message(philo, philo->id, FORK);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->m_lock);
	write_message(philo, philo->id, FORK);
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->death_time);
		write_message(philo, philo->id, DEAD);
		return ;
	}
	pthread_mutex_lock(philo->lfork);
	write_message(philo, philo->id, FORK);
	pthread_mutex_lock(&philo->m_lock);
	write_message(philo, philo->id, EAT);
	philo->nbr_of_meals++;
	philo->eating = 1;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->m_lock);
	ft_usleep(philo->data->eat_time);
	pthread_mutex_lock(&philo->m_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->m_lock);
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (alive(philo))
	{
		eat(philo);
		write_message(philo, philo->id, SLEEP);
		ft_usleep(philo->data->sleep_time);
		write_message(philo, philo->id, THINK);
		ft_usleep(50);
	}
	return (philo);
}

void	start_simulation(t_philo *philos)
{
	pthread_t	monitor_t;
	int			i;

	i = 0;
	if (pthread_create(&monitor_t, NULL, monitor, philos))
		return ;
	while (i < philos->data->philo_count)
	{
		pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
		i++;
	}
	pthread_join(monitor_t, NULL);
	i = 0;
	while (i < philos->data->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:37:34 by zelbassa          #+#    #+#             */
/*   Updated: 2024/07/06 14:53:59 by prizmo           ###   ########.fr       */
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

void	*routine(void *data)
{
	t_philo	*philo_data;

	philo_data = (t_philo *)data;
	pthread_join(philo_data->thread, NULL);
	printf("Hello\n");
	free(philo_data);
	return (0);
}

void	init_data(char **av, int ac, t_data *data)
{
	int	i;

	i = 0;
	data->philo_count = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_sleep = ft_atoi(av[3]);
	data->time_to_think = ft_atoi(av[4]);
	if (ac == 6)
		data->eating_rounds = ft_atoi(av[5]);
}

void	init_forks(pthread_mutex_t *forks, char **av)
{
	//
}

void	init_philos(pthread_mutex_t *forks, t_philo *philo)
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
	init_data(av, ac, &data);
	init_forks(forks, av);
	init_philos(forks, philos);
	return (0);
}
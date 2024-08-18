/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:26:27 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/17 20:23:21 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

// void	set_forks(t_philo *philo, pthread_mutex_t *fork, int i)
// {
// 	if (philo->id % 2 == 0)
// 	{
// 		philo->rfork = &fork[i];
// 		philo->lfork = &fork[(i + 1) % philo->data->philo_count];
// 	}
// 	else
// 	{
// 		philo->lfork = &fork[i];
// 		philo->rfork = &fork[(i + 1) % philo->data->philo_count];
// 	}
// }

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
		// set_forks(&philo[i], fork, i);
		if (philo[i].id % 2 == 0)
		{
			philo[i].rfork = &fork[i];
			philo[i].lfork = &fork[(i + 1) % data->philo_count];
		}
		else
		{
			philo[i].lfork = &fork[i];
			philo[i].rfork = &fork[(i + 1) % data->philo_count];
		}
		philo[i].data = data;
		pthread_mutex_init(&philo[i].m_lock, NULL);
		philo[i].hunger_level = get_time();
		philo[i].last_meal_time = get_time();
		philo[i].first_action = get_time();
		i++;
	}
}

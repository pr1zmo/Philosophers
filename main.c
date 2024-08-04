/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:37:34 by zelbassa          #+#    #+#             */
/*   Updated: 2024/08/04 10:06:17 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_ate(t_philo *philo)
{
	int	i;
	int	k;

	k = 0;
	i = 0;
	while (i < philo->data->philo_count)
	{
		pthread_mutex_lock(&philo[i].m_lock);
		if (philo->data->must_eat_count == philo[i].nbr_of_meals)
			k++;
		pthread_mutex_unlock(&philo[i].m_lock);
		i++;
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
			philo->is_dead = 1;
			philo->data->simulation_end = 1;
			pthread_mutex_unlock(&philo->data->death_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->is_dead || philo->data->simulation_end)
		return (pthread_mutex_unlock(&philo->data->death_lock), 0);
	return (pthread_mutex_unlock(&philo->data->death_lock), 1);
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
	if (!check_data(&data))
		return (1);
	start_simulation(&data, philos);
	destroy_all(NULL, philos, forks);
	return (0);
}

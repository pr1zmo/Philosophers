/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:37:34 by zelbassa          #+#    #+#             */
/*   Updated: 2024/12/07 09:45:48 by zelbassa         ###   ########.fr       */
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
		if (philo[i].nbr_of_meals >= philo->data->must_eat_count)
			k++;
		pthread_mutex_unlock(&philo[i].m_lock);
		i++;
	}
	if (k >= philo->data->philo_count)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		philo->data->simulation_end = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		exit(1);
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
			write_message(&philo[i], DEAD);
			pthread_mutex_lock(&philo->data->death_lock);
			philo->is_dead = 1;
			philo->data->simulation_end = 1;
			return (pthread_mutex_unlock(&philo->data->death_lock), 1);
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
	if (!check_data(&data))
		return (1);
	init_forks(forks, av);
	init_philos(forks, &data, philos);
	start_simulation(&data, philos);
	join_philos(&data, philos);
	destroy_all(philos, forks);
	return (0);
}

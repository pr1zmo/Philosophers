/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:32:50 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/07 10:02:59 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *data)
{
	t_philo	*philos;

	philos = (t_philo *)data;
	while (1)
	{
		if (starved(philos))
			break ;
		if (philos->data->must_eat_count > 0 && all_ate(philos))
			break ;
		ft_usleep(10);
	}
	return (NULL);
}

size_t	set_think_time(t_philo *philo)
{
	size_t	think_time;

	if (philo->data->death_time < philo->data->sleep_time
		+ philo->data->eat_time)
		think_time = 50;
	else if (philo->data->death_time
		- (philo->data->eat_time + philo->data->sleep_time) > 50)
		think_time = ((philo->data->death_time - philo->data->eat_time) / 2);
	else
		think_time = 0;
	return (think_time);
}

void	*routine(void *data)
{
	size_t	think_time;
	t_philo	*philo;

	philo = (t_philo *)data;
	think_time = set_think_time(philo);
	if (philo->id % 2 == 0)
		ft_usleep(think_time);
	while (alive(philo))
	{
		// if (get_time() - philo->last_meal_time > philo->data->death_time / 2)
		// 	ft_usleep(10);
			// pthread_mutex_lock(philo->lfork);
		eat(philo);
		write_message(philo, SLEEP);
		ft_usleep(philo->data->sleep_time);
		write_message(philo, THINK);
		think_time = set_think_time(philo);
		// printf("Philo %d will think for %zu ms\n", philo->id, think_time);
		// printf("Free forks: %d\n", philo->free_forks);
		ft_usleep(think_time);
		if (get_time() - philo->last_meal_time
			>= philo->data->death_time
			|| (philo->data->must_eat_count > 0 && all_ate(philo)))
			break ;
	}
	return (data);
}

void	start_simulation(t_data *data, t_philo *philos)
{
	pthread_t	m_thread;
	int			i;

	i = 0;
	if (pthread_create(&m_thread, NULL, &monitor, philos) != 0)
		return ;
	while (i < data->philo_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			break ;
		i++;
	}
	pthread_join(m_thread, NULL);
	return ;
}

void	join_philos(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

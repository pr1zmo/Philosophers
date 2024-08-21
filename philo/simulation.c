/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:32:50 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/21 12:11:41 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *param)
{
	t_philo	*philos;

	philos = (t_philo *)param;
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

void	*routine(void *data)
{
	size_t	think_time;
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->data->death_time < philo->data->sleep_time
		+ philo->data->eat_time)
		think_time = 50;
	else if (philo->data->death_time
		- (philo->data->eat_time + philo->data->sleep_time) > 50)
		think_time = ((philo->data->death_time - philo->data->eat_time) / 2);
	else
		think_time = 10;
	if (philo->id % 2 == 0)
		ft_usleep(think_time);
	while (alive(philo))
	{
		eat(philo);
		write_message(philo, SLEEP);
		ft_usleep(philo->data->sleep_time);
		write_message(philo, THINK);
		ft_usleep(think_time);
	}
	return (NULL);
}

void	start_simulation(t_data *data, t_philo *philos)
{
	pthread_t	m_thread;
	int			i;

	i = 0;
	pthread_create(&m_thread, NULL, &monitor, philos);
	while (i < data->philo_count)
	{
		pthread_create(&philos[i].thread, NULL, &routine, &philos[i]);
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

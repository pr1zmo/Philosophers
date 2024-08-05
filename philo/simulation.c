/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:32:50 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/05 10:11:52 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor(void *param)
{
    t_philo *philos = (t_philo *)param;
    while (1)
    {
        if (starved(philos))
            break;
        if (all_ate(philos))
            break;
        ft_usleep(50);
    }
    return (NULL);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(50);
	while (alive(philo))
	{
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (data);
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
	i = 0;
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

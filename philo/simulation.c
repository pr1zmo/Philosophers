/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:32:50 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/18 11:08:59 by zelbassa         ###   ########.fr       */
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
        if (philos->data->must_eat_count != -1 && all_ate(philos))
            break;
        ft_usleep(50);
    }
	return (NULL);
}

void	*routine(void *data)
{
	size_t	time_to_think;
	t_philo	*philo;
	int		i;

	i = 0;
	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(50);
	while (alive(philo))
	{
		eat(philo);
		write_message(philo, SLEEP);
		ft_usleep(philo->data->sleep_time);
		// time_to_think = (philo->data->death_time - 
		// (get_time() - philo->last_meal_time) - 
		// philo->data->eat_time) / 2;
		// ft_usleep();
		// printf("death time \"%ld\" - eat time: \"%ld\": %ld\n", philo->data->death_time, philo->data->eat_time, philo->data->death_time - philo->data->eat_time);
		if (philo->data->death_time - (philo->data->eat_time + philo->data->sleep_time) > 50)
			time_to_think = (philo->data->death_time - philo->data->eat_time) / 2;
		else
			time_to_think = 0;
		write_message(philo, THINK);
		ft_usleep(time_to_think);
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
	i = 0;
	pthread_detach(m_thread);
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

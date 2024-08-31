/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:31:14 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/28 21:15:09 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

static	int	valid_time(t_data *data)
{
	if (data->eat_time < 60 || data->sleep_time < 60 || data->death_time < 60
		|| data->eat_time > 10000 || data->death_time > 10000
		|| data->sleep_time > 10000)
		return (0);
	return (1);
}

int	check_data(t_data *data)
{
	if (data->philo_count < 1 || data->philo_count > 200)
		return (printf("Invalid number of philosophers\n"), 0);
	if (!valid_time(data))
		return (printf("Invalid time values\n"), 0);
	if (data->must_eat_count < -1 || data->must_eat_count == 0)
		return (printf("Invalid number of meals\n"), 0);
	return (1);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

int	check_death(t_philo philo)
{
	size_t	last_meal_t;

	last_meal_t = get_time() - philo.last_meal_time;
	pthread_mutex_lock(&philo.m_lock);
	if (last_meal_t > philo.data->death_time
		&& philo.eating == 0)
		return (pthread_mutex_unlock(&philo.m_lock), 1);
	return (pthread_mutex_unlock(&philo.m_lock), 0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

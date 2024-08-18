/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:31:14 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/17 19:48:59 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_data(t_data *data)
{
	if (data->philo_count < 1 || data->philo_count > 200)
	{
		printf("%d\n", data->philo_count);
		return (printf("Invalid number of philosophers\n"), 0);
	}
	if (data->eat_time < 60 || data->sleep_time < 60 || data->death_time < 60)
		return (printf("Invalid time values\n"), 0);
	if (data->must_eat_count < -1)
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
	pthread_mutex_lock(&philo.m_lock);
	if (get_time() - philo.last_meal_time >= philo.data->death_time
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
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:29:20 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/04 12:40:27 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->rfork);
	write_message(philo, FORK);
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->death_time);
		pthread_mutex_unlock(philo->rfork);
		return ;
	}
	pthread_mutex_lock(philo->lfork);
	write_message(philo, FORK);
	write_message(philo, EAT);
	pthread_mutex_lock(&philo->m_lock);
	philo->eating = 1;
	philo->nbr_of_meals += 1;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->m_lock);
	ft_usleep(philo->data->eat_time);
	pthread_mutex_lock(&philo->m_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->m_lock);
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
}

void	think(t_philo *philo)
{
	write_message(philo, THINK);
	ft_usleep(50);
}

void	rest(t_philo *philo)
{
	write_message(philo, SLEEP);
	ft_usleep(philo->data->sleep_time);
}

void	destroy_all(char *str, t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, "\n", 1);
	}
	pthread_mutex_destroy(&philo->data->print_lock);
	pthread_mutex_destroy(&philo->data->death_lock);
	while (i < philo->data->philo_count)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&philo[i].m_lock);
		i++;
	}
}

void	write_message(t_philo *philo, char *str)
{
	size_t	timestamp;

	timestamp = get_time() - philo->first_action;
	pthread_mutex_lock(&philo->data->print_lock);
	if (alive(philo))
	{
		if (ft_strcmp(str, EAT) == 0)
			printf("%s%ld %s%d %s%s🍝\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
		else if (ft_strcmp(str, SLEEP) == 0)
			printf("%s%ld %s%d %s%s💤\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
		else if (ft_strcmp(str, THINK) == 0)
			printf("%s%ld %s%d %s%s🤔\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
		else if (ft_strcmp(str, FORK) == 0)
			printf("%s%ld %s%d %s%s🍴\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
		else
			printf("%s%ld %s%d %s%s💀\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

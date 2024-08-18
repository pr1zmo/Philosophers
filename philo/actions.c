/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:29:20 by prizmo            #+#    #+#             */
/*   Updated: 2024/08/17 20:34:41 by zelbassa         ###   ########.fr       */
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
	philo->hunger_level = get_time() - philo->last_meal_time;
	pthread_mutex_unlock(&philo->m_lock);
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
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
		// printf("%s%ld %s%d %s%s\n", RED BOLD, timestamp, GREEN BOLD, philo->id, WHITE, str);
		printf("%ld %d %s\n", timestamp, philo->id, str);
	pthread_mutex_unlock(&philo->data->print_lock);
}

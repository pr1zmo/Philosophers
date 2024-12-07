/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 12:29:20 by prizmo            #+#    #+#             */
/*   Updated: 2024/12/07 09:59:53 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->rfork);
	philo->free_forks -= 1;
	write_message(philo, FORK);
	if (philo->data->philo_count == 1)
	{
		ft_usleep(philo->data->death_time);
		pthread_mutex_unlock(philo->rfork);
		return ;
	}
	pthread_mutex_lock(philo->lfork);
	philo->free_forks -= 1;
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
	philo->free_forks += 1;
	pthread_mutex_unlock(philo->rfork);
	philo->free_forks += 1;
}

void	destroy_all(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
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
		printf("%s%ld %s%d %s%s\n", RED BOLD, timestamp,
			GREEN BOLD, philo->id, WHITE, str);
	pthread_mutex_unlock(&philo->data->print_lock);
}

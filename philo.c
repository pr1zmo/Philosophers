/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prizmo <prizmo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:36:06 by prizmo            #+#    #+#             */
/*   Updated: 2024/07/01 13:02:56 by prizmo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char	const *c)
{
	int	i = 0;
	while (c[i]) {
		if (c[i] >= '0' && c[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					sign;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = (-1 * (*str == '-')) + (*str == '+');
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result > (unsigned long long)LLONG_MAX && sign == 1)
			return (-1);
		else if (result > (unsigned long long)LLONG_MAX && sign == -1)
			return (0);
		str++;
	}
	return ((int)(result * sign));
}

int	*check_params(char **av, int const ac) {
	int	i;
	int	*result;

	i = 0;
	result = malloc(sizeof(int) * ac - 1);
	while (i < ac - 1) {
		if (ft_isdigit(av[i + 1]))
			result[i] = atoi(av[i + 1]);
		else {
			free(result);
			return (0);
		}
		i++;
	}
	return (result);
}

void	init_philos(t_philo	*philo, char **av, int const ac)
{
	t_data	*data;
	int		*output;

	output = check_params(av, ac);
	if (!output[0]) {
		free(output);
		printf("%s\n", ERR_IN_2);
		return ;
	}
}

size_t	get_time()
{
	size_t			curr_time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	curr_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (curr_time / 10);
}

void	*routine()
{
	printf("Hello\n");
	return (0);
}

int	main(int ac, char **av) {
	t_philo	*philos;

	philos = NULL;
	if (ac != 5 && ac != 6) {
		printf("%s\n", ERR_IN_3);
		return (0);
	}
	else
		init_philos(philos, av, ac);
	printf("ALL GOOD: %ld\n", get_time());
	return (1);
}
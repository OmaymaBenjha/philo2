/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:07:20 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/19 12:26:29 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
int	ft_isalpha(const char *av)
{
	int	i;
	
	i = 0;
	while (av[i])
	{
		if (!(av[i] >= '0' && av[i] <= '9'))
			return (0);
		i++;
 	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isalpha(str))
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(result * sign));
}
void	accurate_usleep(long time_ms)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_ms)
		usleep(69);
}
void print_state(t_philosopher *p, const char *state)
{
	long	tstp;

    pthread_mutex_lock(&p->sim->output_mutex);
    if (!stop_getter(p->sim))
	{
		tstp = get_time_ms() - p->sim->start_time;
		printf("%ld %d %s\n", tstp, p->id, state);
	}
    pthread_mutex_unlock(&p->sim->output_mutex);
}

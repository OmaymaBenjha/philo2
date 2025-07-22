/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:31:35 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/22 22:19:50 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*main_loop(t_philosopher *philos, t_simulation *sim)
{
	
	int		i;
	long	last_meal;

	i = 0;
	while (i < sim->num_of_philo)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		last_meal = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		if ((get_time_ms() - last_meal) > sim->time_to_die
			&& !stop_getter(sim))
		{
			pthread_mutex_lock(&sim->output_mutex);
            if (!sim->stop)
            {
				// printf("last meal at %ld\n", last_meal - sim->start_time);
                printf("%ld %d died\n", get_time_ms() - sim->start_time, philos[i].id);
				stop_setter(sim);
            }
            pthread_mutex_unlock(&sim->output_mutex);
            return (NULL);
		}
		accurate_usleep(100, &sim->philos[i]);
		i++;
	}
	return ((void *)1);
}

static void	*meals_check(t_simulation *sim, t_philosopher *philos)
{
	int	i;
	int	all_done;

	if (sim->must_eat_count == -1)
		return 0;
	i = 0;
	all_done = 1;
	while (i < sim->num_of_philo)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals_eaten < sim->must_eat_count && !stop_getter(sim))
			all_done = 0;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		if (!all_done)
			break ;
		i++;
	}
	if (all_done)
	{
		pthread_mutex_lock(&sim->output_mutex);
		stop_setter(sim);
		pthread_mutex_unlock(&sim->output_mutex);
		return (NULL);
	}
	return ((void *)1);
}

static void	*monitor_routine(void *arg)
{
	t_philosopher	*philos;
	t_simulation	*sim;

	philos = (t_philosopher *)arg;
	sim = philos[0].sim;
	while (!stop_getter(sim))
	{
		if (!main_loop(philos, sim))
			break ;
		if (sim->must_eat_count > 0)
		{
			if (!meals_check(sim, philos))
				break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	monitor_trigger(t_simulation *sim)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	pthread_create(&monitor, NULL, monitor_routine, sim->philos);
	while (i < sim->num_of_philo)
	{
		pthread_join(sim->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

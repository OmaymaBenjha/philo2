/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 23:22:14 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/22 21:24:40 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void locking_mechanism(t_philosopher *p)
{
    // pthread_mutex_t *first;
    // pthread_mutex_t *second;
    
    // if (p->id % 2 == 0)
    // {
    //     first = p->l_fork;
    //     second = p->r_fork;
    // }
    // else
    // {
    //     first = p->r_fork;
    //     second = p->l_fork;
    // }
    pthread_mutex_lock(p->l_fork);
    print_state(p, "has taken a fork");
    pthread_mutex_lock(p->r_fork);
    print_state(p, "has taken a fork");
    pthread_mutex_lock(&p->meal_mutex);
    print_state(p, "is eating");
    p->last_meal_time = get_time_ms();
    p->meals_eaten++;
    pthread_mutex_unlock(&p->meal_mutex);
    accurate_usleep(p->sim->time_to_eat, p);
    pthread_mutex_unlock(p->r_fork);
    pthread_mutex_unlock(p->l_fork);    
}

void    *routine(void *data)
{
    t_philosopher   *p;
    
    p = (t_philosopher *)data;
    if (p->sim->num_of_philo == 1)
	{
		print_state(p, "has taken a fork");
		accurate_usleep(p->sim->time_to_die, p);
		return (NULL);
	}
    if (p->id % 2 == 0)
		accurate_usleep(p->sim->time_to_eat, p);
    while (!stop_getter(p->sim))
    {
        locking_mechanism(p);
        if (p->sim->must_eat_count > 0)
        {
            pthread_mutex_lock(&p->meal_mutex);
            if (p->meals_eaten >= p->sim->must_eat_count)
            {
                pthread_mutex_unlock(&p->meal_mutex);
                break ;
            }
            pthread_mutex_unlock(&p->meal_mutex);
        }
        print_state(p, "is sleeping");
        accurate_usleep(p->sim->time_to_sleep, p);
        print_state(p, "is thinking");
    }
    return (NULL);
}

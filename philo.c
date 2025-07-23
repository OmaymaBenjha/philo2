/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:04:12 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/22 17:28:58 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_entry(int ac, char **av)
{
    if (ac != 5 && ac != 6)
    {
        printf("Usage: %s num_philos t_die t_eat t_sleep [must_eat]\n", av[0]);
        return (0);
    }
    return (1);
}
static void clean_up(t_simulation *sim)
{
    int i;
    
    i = 0;
 
    while (i < sim->num_of_philo)
    {
        pthread_mutex_destroy(&sim->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&sim->stop_mutex);
    pthread_mutex_destroy(&sim->output_mutex);
    i = 0;
    while (i < sim->num_of_philo)
    {
        pthread_mutex_destroy(&sim->philos[i].meal_mutex);  
        i++;
    }
}
int main(int ac, char **av)
{
    t_simulation    sim;

    if (!check_entry(ac, av))
        return (0);
    memset(&sim, 0, sizeof(sim));
    if (!init_simulation(&sim, ac, av))
        return (printf("Invalid Arguments!\n"), 1);
    if (!init_philo(&sim)
        return (1);
    simulation_trigger(&sim);
    monitor_trigger(&sim);
    clean_up(&sim);
    return (0);
}

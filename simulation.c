/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:59:13 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/22 20:32:09 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_simulation(t_simulation *sim, int ac, char **av)
{
    int i;

    i = 0;
    sim->num_of_philo = ft_atoi(av[1]);
    sim->time_to_die = ft_atoi(av[2]);
    sim->time_to_eat = ft_atoi(av[3]);
    sim->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        sim->must_eat_count = ft_atoi(av[5]);
    else
        sim->must_eat_count = -1;
    if (sim->num_of_philo <= 0 ||
        sim->num_of_philo > MAX_PHILO || sim->time_to_die <= 0 ||
            sim->time_to_eat <= 0 || sim->time_to_sleep <= 0 ||
                (ac == 6 && sim->must_eat_count <= 0))
                return (0);
    pthread_mutex_init(&sim->stop_mutex, NULL);
    pthread_mutex_init(&sim->output_mutex, NULL);
    while (i < sim->num_of_philo)
    {
        pthread_mutex_init(&sim->forks[i], NULL);
        i++;
    }
    return (1);
}
long get_time_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void    init_philo(t_simulation *sim)
{
    int i;

    i = 0;
    sim->start_time = get_time_ms();
    while (i < sim->num_of_philo)
    {
        sim->philos[i].id = i + 1;
        sim->philos[i].sim = sim;
        sim->philos[i].l_fork = &sim->forks[i];
        sim->philos[i].r_fork = &sim->forks[(i + 1) % sim->num_of_philo]; 
        pthread_mutex_init(&sim->philos[i].meal_mutex, NULL);          
        sim->philos[i].last_meal_time = sim->start_time;
        i++;
    }
}
void    simulation_trigger(t_simulation *sim)
{
    int i;

    i = 0;
    while (i < sim->num_of_philo)
    {
        pthread_create(&sim->philos[i].thread, NULL, routine, &sim->philos[i]);
        i++;
    }
}

//27119 147 is sleeping 
//27419 147 is thinking ======> 27418 147 died


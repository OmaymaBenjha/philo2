/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:04:12 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/19 14:41:18 by oben-jha         ###   ########.fr       */
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

int main(int ac, char **av)
{
    t_simulation    sim;

    if (!check_entry(ac, av))
        return (0);
    ft_memset(&sim, 0, sizeof(sim));
    if (!init_simulation(&sim, ac, av))
        return(printf("Invalid Arguments!\n"), 1);
    init_philo(&sim);
    simulation_trigger(&sim);
    monitor_trigger(&sim);
    return (0);
}

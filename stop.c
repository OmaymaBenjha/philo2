/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 12:29:07 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/22 17:28:35 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int stop_getter(t_simulation *sim)
{
    int result;
    
    pthread_mutex_lock(&sim->stop_mutex);
    result = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return (result);
}

void stop_setter(t_simulation *sim)
{
    pthread_mutex_lock(&sim->stop_mutex);
    sim->stop = 1;
    pthread_mutex_unlock(&sim->stop_mutex);
}
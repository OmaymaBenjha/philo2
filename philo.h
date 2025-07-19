/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:55:11 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/19 17:42:24 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define MAX_PHILO 200

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
# include <limits.h>

struct s_simulation;

typedef struct s_philosopher
{
    int                 id;
    pthread_t           thread;
    pthread_mutex_t     *l_fork;
    pthread_mutex_t     *r_fork;
    long                last_meal_time;
    int                 meals_eaten;
    struct s_simulation *sim;
    pthread_mutex_t     meal_mutex;
} t_philosopher;

typedef struct s_simulation
{
    int             num_of_philo;
    long             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    int             stop;
    long            start_time;
    pthread_mutex_t forks[MAX_PHILO];
    pthread_mutex_t stop_mutex;
    pthread_mutex_t output_mutex;
    t_philosopher   philos[MAX_PHILO];
} t_simulation;

//-----------------------simulation
int     init_simulation(t_simulation *sim, int ac, char **av);
void    init_philo(t_simulation *sim);
void    simulation_trigger(t_simulation *sim);
void    *routine(void *data);
int     stop_getter(t_simulation *sim);
void    stop_setter(t_simulation *sim);
long    get_time_ms(void);
void	accurate_usleep(long time_ms, t_philosopher *p);
void    monitor_trigger(t_simulation *sim);

//-----------------------tools
void	*ft_memset(void *b, int c, size_t len);
int	    ft_atoi(const char *str);
void    print_state(t_philosopher *p, const char *state);
int     ft_isalpha(const char *av);



#endif
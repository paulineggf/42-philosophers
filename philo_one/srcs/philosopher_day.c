/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:16:34 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:37:53 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

unsigned long	get_time(void)
{
	struct timeval	tv;
	unsigned long	ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_usec;
	ret /= 1000;
	ret += (tv.tv_sec * 1000);
	return (ret);
}

void        ft_usleep(long int us, t_philo *philo)
{
    struct timeval start;
    struct timeval now;
    
    gettimeofday(&now, NULL);
	start = now;
    while (((now.tv_sec - start.tv_sec) * 1000000)
        + ((now.tv_usec - start.tv_usec)) < us)
    {
        gettimeofday(&now, NULL);   
		if (((now.tv_usec / 1000) + now.tv_sec * 1000) - philo->diying > philo->set->time_to_die)
		{
			print_message(philo, DIED);
			return ;
		}
        usleep(1);
    }
}

int				philosopher_nap(t_philo *philo)
{
	if (!(print_message(philo, SLEEP)))
		return (0);
	ft_usleep(philo->set->time_to_sleep * 1000, philo);
	if (!(print_message(philo, THINK)))
		return (0);
	return (1);
}

int				kill_program(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->set->lock[philo->right]));
	pthread_mutex_unlock(&(philo->set->lock[philo->left]));
	return (0);
}

int				philosopher_meal(t_philo *philo)
{
	// if (!(print_message(philo, FORK)))
	// 	return (kill_program(philo));
	if (!(print_message(philo, EAT)))
		return (kill_program(philo));
	philo->diying = get_time();
	ft_usleep(philo->set->time_to_eat * 1000, philo);
	pthread_mutex_unlock(&(philo->set->lock[philo->right]));
	pthread_mutex_unlock(&(philo->set->lock[philo->left]));
	philo->eating = 0;
	philo->set->fork[philo->right] = 1;
	philo->set->fork[philo->left] = 1;
	philo->time_must_eat += 1;
	if (philo->set->number_of_philosopher != -1 &&
	philo->time_must_eat ==
	philo->set->number_of_time_each_philosophers_must_eat)
		return (0);
	return (1);
}

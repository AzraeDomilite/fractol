/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blucken <blucken@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 17:24:47 by blucken           #+#    #+#             */
/*   Updated: 2024/10/04 17:24:47 by blucken          ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	index;

	index = 0;
	while (lst)
	{
		lst = lst->next;
		index ++;
	}
	return (index);
}

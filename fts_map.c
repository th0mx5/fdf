/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thbernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:16:39 by thbernar          #+#    #+#             */
/*   Updated: 2018/03/08 14:07:10 by thbernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_map_init(t_map *map, char *file_name)
{
	map->fname = ft_strdup(file_name);
	map->zoom = 1.00;
	map->fsize.y = 0;
	map->fsize.x = 0;
	map->fsize.z = 0;
	map->maxvalue = 0;
	map->xshift = 0;
	map->yshift = 0;
	map->zshift = 0;
	ft_map_counts(map);
	if (map->fsize.z != map->fsize.x * map->fsize.y)
		ft_error("error : invalid files\n");
	map->wsize.x = map->fsize.x * 40;
	map->wsize.y = map->fsize.y * 40;
	if (map->wsize.x < 50)
		map->wsize.x = 50;
	if (map->wsize.y < 50)
		map->wsize.y = 50;
	if (map->wsize.x > 1000)
		map->wsize.x = 1000;
	if (map->wsize.y > 1000)
		map->wsize.y = 1000;
	ft_map_allocvalues(map);
	ft_map_writevalues(map);
}

void	ft_map_counts(t_map *map)
{
	char	*s;
	int		i[3];
	char	**array;

	if (((i[1] = open(map->fname, O_RDONLY)) < 0))
		ft_error("error : invalid file\n");
	while ((i[2] = get_next_line(i[1], &s)) > 0)
	{
		i[0] = 0;
		map->fsize.y++;
		map->fsize.x = 0;
		array = ft_strsplit(s, ' ');
		free(s);
		while (array[i[0]])
		{
			if (ft_atoi(array[i[0]]) > map->maxvalue)
				map->maxvalue = ft_atoi(array[i[0]]);
			map->fsize.x++;
			i[0]++;
			map->fsize.z++;
		}
		ft_free_strsplit(array);
	}
	if (i[2] != -1)
		free(s);
}

int		ft_map_allocvalues(t_map *map)
{
	map->fsize.z = map->fsize.y * map->fsize.x;
	map->values = (t_coord*)malloc(sizeof(t_coord) * map->fsize.z);
	if (map->values == NULL)
		exit(-1);
	return (0);
}

void	ft_map_writevalues(t_map *map)
{
	int		j;
	int		i;
	int		fd;
	char	**array;
	char	*s;

	i = 0;
	if (((fd = open(map->fname, O_RDONLY)) < 0))
		ft_error("error : invalid file\n");
	while ((get_next_line(fd, &s)) > 0)
	{
		j = 0;
		array = ft_strsplit(s, ' ');
		free(s);
		while (j < map->fsize.x)
		{
			map->values[(i * map->fsize.x) + j].z = ft_atoi(array[j]);
			map->values[(i * map->fsize.x) + j].y = i;
			map->values[(i * map->fsize.x) + j].x = j;
			j++;
		}
		i++;
		ft_free_strsplit(array);
	}
	free(s);
}

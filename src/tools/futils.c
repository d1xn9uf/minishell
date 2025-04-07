#include "../../inc/tools.h"

char		*readfile_join(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*res;

	if (!s1)
		return (minishell_strdup(s2));
	if (!s2)
		return (minishell_strdup(s1));
	total_len = minishell_strlen(s1) + minishell_strlen(s2);
	res = (char *)malloc(sizeof(char) * (total_len + 1));
	if (res == NULL)
		return (NULL);
	minishell_strlcpy(res, s1, minishell_strlen(s1) + 1);
	minishell_strlcat(res, s2, total_len + 1);
	return (res);
}

char	*minishell_readfile(char *filename)
{
	int32_t	fd;
	int32_t	rbytes;
	char	buffer[FREAD_SIZE + 1];
	char	*data;
	char	*p;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (NULL);
	rbytes = FREAD_SIZE;
	data = NULL;
	while (rbytes >= FREAD_SIZE)
	{
		rbytes = read(fd, &buffer, FREAD_SIZE);
		buffer[rbytes] = 0;
		p = data;
		data = readfile_join(data, buffer);
		if (p)
		 	free(p);
	}
	return (data);
}

void	generate_filename(int32_t fd, char *filename)
{
	int32_t	rbytes;
	int32_t	i;

	rbytes = read(fd, filename, FNAME_SIZE);
    if (rbytes != FNAME_SIZE)
	{
        perror("Error reading random bytes");
        free(filename);
        close(fd);
        return ;
    }
	minishell_strlcpy(filename, "/tmp/.", 7);
	i = 6;
    while (i < FNAME_SIZE)
	{
		filename[i] = "0123456789ABCDEF"[(uint32_t)filename[i] % 16];
		i++;
    }
    filename[i] = 0; 
    close(fd);
}

/* 
 * generate a random file name
 * path : /tmp
*/
char	*minishell_generate_filename()
{
	int32_t	fd;
	char	*filename;

	fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) 
	{
        perror("Error opening /dev/urandom");
        return (NULL);
    }
    filename = (char *)minishell_calloc(1, FNAME_SIZE + 1);
    if (filename == NULL) 
	{
        perror("Memory allocation failed");
        close(fd);
        return (NULL);
    }
    generate_filename(fd, filename);
	return (filename);
}

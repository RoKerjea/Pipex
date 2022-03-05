int ft_strchr(const char *s, char t)
{
    int i;

    i = 0;
    while(s[i] && s[i] != t)
        i++;
    if(s[i])
        return (1);
    return (0);
}
--Write a query to list the names of all people who starred in a movie in 2004, ordered by birth year
SELECT DISTINCT c.name
FROM movies a
INNER JOIN stars B
ON a.id = b.movie_id
INNER JOIN people c
ON b.person_id = c.id
WHERE a.year = 2004
ORDER BY c.birth asc;
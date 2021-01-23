--Names of all directors with a movie rated a 9.0 or higher
SELECT DISTINCT c.name
FROM movies a
INNER JOIN directors B
ON a.id = b.movie_id
INNER JOIN people c
ON b.person_id = c.id
INNER JOIN ratings d
on a.id = d.movie_id
WHERE d.rating >= 9.0;
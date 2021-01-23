--Top five rated movies for Chadwick Boseman
SELECT a.title
FROM movies a
INNER JOIN stars B
ON a.id = b.movie_id
INNER JOIN people c
ON b.person_id = c.id
INNER JOIN ratings d
on a.id = d.movie_id
WHERE c.name = 'Chadwick Boseman'
ORDER BY d.rating DESC
LIMIT 5;
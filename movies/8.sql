--List the names of the stars from Toy Story
SELECT c.name
FROM movies a
INNER JOIN stars B
ON a.id = b.movie_id
INNER JOIN people c
ON b.person_id = c.id
WHERE a.title = 'Toy Story';
--Average rating of all movies released in 2012
SELECT AVG(b.rating) as "Average Rating"
FROM movies a
INNER JOIN ratings b
ON a.id = b.movie_id
WHERE a.year = 2012;
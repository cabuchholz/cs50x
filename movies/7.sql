--List all movies from 2010 with rating, descending order by rating then title (ascending)
SELECT a.title
    , b.rating
FROM movies a
INNER JOIN ratings b --only keep movies with ratings, so inner join
ON a.id = b.movie_id
WHERE year = 2010
ORDER BY rating desc, title asc;
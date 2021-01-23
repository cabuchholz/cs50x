--Query to return all movies from 2018 or later, in alpabetical order
SELECT title
FROM movies
WHERE year >= 2018
ORDER BY title asc;
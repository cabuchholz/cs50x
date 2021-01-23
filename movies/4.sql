--Determine the number of movies with IMDB rating of 10.0
SELECT COUNT(*) as 'Movies with rating of 10'
FROM ratings
WHERE rating = 10.0;
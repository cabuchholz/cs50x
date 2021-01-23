--Query to show all of the movies that Johnny Deep and Helena Bonham were in together

WITH jd as
(
SELECT a.movie_id
FROM stars a
INNER JOIN people b
on a.person_id = b.id
WHERE b.name = 'Johnny Depp'
),

hc as
(
SELECT a.movie_id
FROM stars a
INNER JOIN people b
on a.person_id = b.id
WHERE b.name = 'Helena Bonham Carter'
)

SELECT a.title
FROM movies a
WHERE a.id in (SELECT b.movie_id from jd b)
AND a.id in (SELECT c.movie_id from hc c);
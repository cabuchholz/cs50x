--Query to show all of the co-stars of Kevin Bacon

WITH kb_movies as
(
SELECT a.movie_id
FROM stars a
INNER JOIN people b
on a.person_id = b.id
WHERE b.name = 'Kevin Bacon'
AND b.birth = 1958
),

kb_id as
(
SELECT a.id
FROM people a
WHERE a.name = 'Kevin Bacon'
AND a.birth = 1958
)

SELECT DISTINCT a.name
FROM people a
INNER JOIN stars b
ON a.id = b.person_id
WHERE b.movie_id IN (SELECT c.movie_id from kb_movies c)
AND b.person_ID NOT IN (SELECT d.id from kb_id d);
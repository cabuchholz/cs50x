--List all titles and release years of the Harry Potter movies, in chronological order
SELECT title
    , year
FROM movies
WHERE title like 'Harry Potter%'
ORDER BY year asc;
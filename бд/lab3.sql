--3. Retrieve the first and last names of staff from staff table who are currently active but do not have a picture stored

SELECT first_name, last_name FROM staff
WHERE active = true AND picture IS NULL;

--4.List the titles of all films from the film table with their lengths, but only include films longer than 50 minutes. Sort the results by length from longest to shortest.

SELECT title, length FROM film
WHERE length > 50
ORDER BY length DESC;

--5.Find all films with a length between 100 and 110 minutes that have a rating of 'G' from the film table. The result should be ordered by length in ascending order

SELECT title,rating,length FROM film
WHERE length>100 AND length<110 AND rating = 'G';
ORDER BY length ASC;

--6.Show title, rating, length. Sort by rating A to Z, and inside each rating by length from longest to shortest

SELECT title, rating, length FROM film
ORDER BY rating ASC,length DESC;

--7.From the rental table, show rental_id and return_date. Newest return_date first, and rentals not returned yet (NULL) at the end.

SELECT rental_id, return_date FROM rental
ORDER BY return_date DESC NULLS LAST;

--8.Find the minimum film length for every rating in the film table

SELECT rating, MIN(length) as min_length FROM film
GROUP BY rating;

--9. Show the film title in upper case and the film length in hours. Name the columns title_upper and length_hours.

SELECT UPPER(title) as title_upper, length/60.0 as length_hours FROM film

--10.Show the longest film for each rating (title, rating, length). If two films have the same length, show the first title in A to Z order. (Hint: use DISTINCT ON)

SELECT title, rating, length FROM film
ORDER BY rating ASC, length DESC, title ASC;

--11.Find each unique set of special_features with the total sum of rental rates of all films from the film table that have those features

SELECT special_features, SUM(rental_rate) as total_rental_rate
FROM film
GROUP BY special_features;

--12.Find the number of films in each rating category from the film table. Only display the categories that have more than 200 films.

SELECT rating, COUNT(*) as film_count
FROM film
GROUP BY rating
HAVING COUNT(*)>200;

--13.Find all first names and last names of the actor and staff from actor and staff tables. Use UNION.

SELECT first_name, last_name FROM actor
UNION
SELECT first_name, last_name FROM staff;

--14.Do Q13 again with UNION ALL. Which one gives more rows, UNION or UNION ALL?

SELECT first_name, last_name FROM actor
UNION ALL
SELECT first_name, last_name FROM staff;

--15.Select the first name and last name of actors who do not exist in the staff table

SELECT first_name, last_name FROM actor
WHERE NOT EXISTS(
    SELECT 1 FROM staff
    WHERE staff.first_name = actor.first_name AND staff.last_name = actor.last_name
);

--16.Using INTERSECT, list the titles of films with length greater than 100 and replacement_cost less than 25

SELECT title FROM film
WHERE length > 100
INTERSECT
SELECT title FROM film
WHERE replacement_cost < 25; 

--17.Show each different rating value from the film table

SELECT DISTINCT rating FROM film;

--18.Skip the first 10 longest films and show the next 5. If two films have the same length, sort by title A to Z.

SELECT title, length
FROM film
ORDER BY length DESC, title ASC
LIMIT 5, OFFSET 10;

--19.Count the films for each rating, but only films longer than 100 minutes. Show only the ratings that have more than 110 such films

SELECT rating, COUNT(*) as film_count
FROM film
WHERE length > 100
GROUP BY rating
HAVING COUNT(*) > 110;
-- Lab 4: Function and Operator
-- Deadline week 6
-- Using Sakila database from lab 3
-- Note: Connect to faculty database before running these queries

-- 1. Using Sakila database from lab 3, write your queries and save in lab4.sql file.

-- 2. Select the first_name in uppercase and the last_name in lowercase for all actors in the actor table.
SELECT UPPER(first_name) AS first_name_upper, LOWER(last_name) AS last_name_lower
FROM actor;

-- 3. List title and rental_rate of every film, along with a computed column rental_rate_with_tax equal to rental_rate plus 10% tax.
SELECT title, rental_rate, rental_rate * 1.10 AS rental_rate_with_tax
FROM film;

-- 4. Display the first 4 characters of each film's title from the film table.
SELECT LEFT(title, 4) AS title_first_4_chars
FROM film;

-- 5. List customer_id, first_name and last_name of every customer whose customer_id is between 100 and 200, inclusive.
SELECT customer_id, first_name, last_name
FROM customer
WHERE customer_id BETWEEN 100 AND 200;

-- 6. List title and rating for every film whose rating is 'PG' or 'PG-13'.
SELECT title, rating
FROM film
WHERE rating IN ('PG', 'PG-13');

-- 7. Combine first_name and last_name from the customer table into a single column named full_name, separated by a space (e.g., 'MARY SMITH').
SELECT CONCAT(first_name, ' ', last_name) AS full_name
FROM customer;

-- 8. Using CONCAT(), return a single column film_label formatted as 'title (release_year)' for every film.
SELECT CONCAT(title, ' (', release_year, ')') AS film_label
FROM film;

-- 9. Select the customer_id, first_name, last_name, and email from the customer table. Use COALESCE() to display 'no email provided' for any customer whose email is NULL.
SELECT customer_id, first_name, last_name, COALESCE(email, 'no email provided') AS email
FROM customer;

-- 10. Select all films from the film table whose title contains the word 'star' using case-insensitive matching.
SELECT *
FROM film
WHERE LOWER(title) LIKE '%star%';

-- 11. Find customer_id and first_name of every customer whose first_name has 'a' as its second letter (pattern '_a%').
SELECT customer_id, first_name
FROM customer
WHERE first_name LIKE '_a%';

-- 12. Use the FORMAT() function on the customer table to display each customer as: "Customer [id]: [first_name] [last_name] (Email: [email])". If the email is NULL, show "no email provided".
SELECT format('Customer %s: %s %s (Email: %s)', customer_id, first_name, last_name, COALESCE(email, 'no email provided')) AS customer_info
FROM customer;

-- 13. Select all film titles and their character lengths from the film table where the title length is greater than 20 characters.
SELECT title, LENGTH(title) AS title_length
FROM film
WHERE LENGTH(title) > 20;

-- 14. Return, in a single row, the absolute difference between the maximum and minimum film length across all films, aliased as length_range.
SELECT ABS(MAX(length) - MIN(length)) AS length_range
FROM film;

-- 15. Return title and the square root of replacement_cost for every film, aliased as sqrt_cost.
SELECT title, SQRT(replacement_cost) AS sqrt_cost
FROM film;

-- 16. Calculate the hourly rental rate for each film in the film table (rental_rate / (rental_duration * 24)). Round the resulting rate to 2 decimal places using ROUND().
SELECT title, ROUND(rental_rate / (rental_duration * 24), 2) AS hourly_rental_rate
FROM film;

-- 17. Return rental_id and rental_date of every rental alongside the current date (CURRENT_DATE), aliased as report_date.
SELECT rental_id, rental_date, CURRENT_DATE AS report_date
FROM rental;

-- 18. Return rental_id, rental_date and the hour portion of rental_date (using EXTRACT), aliased as rental_hour.
SELECT rental_id, rental_date, EXTRACT(HOUR FROM rental_date) AS rental_hour
FROM rental;

-- 19. From the rental table, calculate the total duration of each rental using AGE(return_date, rental_date) for all returned items.
SELECT rental_id, rental_date, return_date, AGE(return_date, rental_date) AS rental_duration
FROM rental
WHERE return_date IS NOT NULL;

-- 20. Classify films in the film table into length categories using CASE WHEN: length < 60 -> 'Short', length BETWEEN 60 AND 120 -> 'Medium', length > 120 -> 'Long'.
SELECT title, length,
    CASE
        WHEN length < 60 THEN 'Short'
        WHEN length BETWEEN 60 AND 120 THEN 'Medium'
        WHEN length > 120 THEN 'Long'
    END AS length_category
FROM film;

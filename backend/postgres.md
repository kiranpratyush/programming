## Document all the learning experience here (Once the Md file is substantially large put it into the Obsidian)

1. Keep this document well documented iteratively so that you can come here and come upto the speed in no time
2. Don't jump between topics, even if you do come here and update what ever your learnt

### What are the concepts needs to be covered.

- Just cover what ART of Postgres does
- Give instruction what you want in a declarative way in contrast to imperative programming language.
  -After that, the RDBMS of your choice is going to figure out a plan then execute it, and hopefully return just the result set you wanted!

DML Commands
Insert into
Delete where
Update set
DDL Commands
Create table
alter table
drop table
TCL Commands
begin transaction
commit
rollback
savepoint
prepare commit
commit prepared
rollback prepared
DCL commands
grant
revoke
Postgres maintenance commands
vacuum
analyse
cluster
prepare
execute
explain
listen
notify
lock
set

### Select command (Projection operator)

- In Postgres "FROM" is optional, you can use like select 1; or select now(); etc
- Fetch first n rows only : Is an official standard of sql of limit N.
- Why it is not recommended to do select \*
  - What is a Toast mechanism in postgres https://www.postgresql.org/docs/current/storage-toast.html understand and write something about it , take x premium tomorrow sunday 30th august 2026
  - It is better to be explicit on the name of the column getting retrieved
- Visiblity rule of select alias (select name as name2) name2 is an alias.
  - FROM, WHERE,GROUP BY, HAVING,JOIN : is executed first before SELECT. hence alias is not available here
  - ORDER BY, LIMIT/FETCH: is executed after the select clause to order or limit the resultant rows.

- Different function and operators available in postgres
  - (TODO) later get a glimpse of date/time functions and operators
- I was thinking SELECT and FROM part of same clause, but now think that FROM is a different clause -> where -> groupby->having->join-> select -> orderby
- order by has nulls first or nulls last
- Postgres also provides distinct on clause to choose columns on which entries will be distinct.
- With out ORDER BY DISTINCT ON becomes unpredictable.
- (TODO) https://www.postgresql.org/docs/current/queries-table-expressions.html#QUERIES-FROM this is next read to understand join,from clause etc.
- NULL does not mean nothing , It means UNKNOWN, whatever comparison with UNKNOWN produces UNKNOWN
- I have to start reading from WHERE again at least to understand the the not exists , what is the distinction of where , how it works as a filter.
- That said we always try to keep the where clause simple to make sure the indexing is used properly (What does it mean).
- with Where we can add and clause so that when the first condition fails it get's short circuted.
- SQL uses three valued logic (true,false,unknown)
- not exists just filters if the row was selected or not. You can simply select anything may be select 1 from table where some condition , then not exists filters the row out
- You can use order by with custom conditions like custom comparitor in c++ using CASE when status ='Power failure' then 1 else 2
- Try to understand the explain command and try to read the query planner execution
- What is table inheritance hierarchy
- This is equivalent of inheritance of OOP . You can create a parent table and child table can inherit from the parent table
- You can put additional constrain on join also for example join on x.num=y.num and y.value = 3;

## TODOS

https://www.postgresql.org/docs/current/storage-toast.html (Why it is not recommended to do select \*)
Why not to use offset,and how to implement pagination
Lateral join query what is does
Indexing

## Postgres chapter 7 (september 1st to 4th)

### Implementation progress

#### Practice database

- Created the schema manually for `customers`, `orders`, and `order_items`.
- Seeded 20 customers, 100 orders, and 200 order items.
- Kept four customers without orders so that outer-join behavior could be tested.
- Used constraints, foreign keys, identity columns, and indexes in the schema.

#### Exercise 1: Fetch orders belonging to a customer

Implemented `GET /customers/{customerId}/orders` using C# and `Npgsql`.

Concepts practiced:

- `INNER JOIN` between `orders` and `order_items`.
- Filtering rows by customer and an optional starting date.
- Grouping line items by order.
- Calculating an order total with `SUM(quantity * unit_price)`.
- Returning explicit columns and aliases instead of `SELECT *`.
- Deterministic ordering and an optional `LIMIT`.
- Supplying values through Npgsql parameters rather than interpolating them into SQL.
- Building the SQL conditionally and assigning the completed SQL to `CommandText`.
- Reading result columns with `NpgsqlDataReader` and mapping them to a C# response model.
- Returning an empty collection when a customer has no orders.

Verified behavior:

- Customer 1 returned seven orders with their calculated totals.
- Customer 20 returned an empty collection.
- The date and limit parameters changed the result as expected.

#### Exercise 2: Summarize orders for every customer

Implemented `GET /customers/order-summary` with an optional status filter.

The response contains one row per customer with the order count, total spending, and most recent order date.

Concepts practiced:

- `LEFT JOIN` preserves customers even when no matching order exists.
- A derived table can prepare a result set before it is joined to another table.
- Filtering the orders inside the derived table preserves customers without matching orders. Applying that filter carelessly after the outer join can remove the null-extended customer rows.
- Joining an order to `order_items` changes the result grain from one row per order to one row per order item.
- Because of that grain change, `COUNT(result.id)` counts line items rather than orders.
- `COUNT(DISTINCT result.id)` counts each order once while leaving all item rows available for calculating total spending.
- An alternative design is to aggregate order items inside the derived table first, producing one row per order, and then aggregate those order rows per customer.
- `COALESCE` converts the null sum for a customer with no matching orders to zero.
- `MAX(placed_at)` obtains the latest matching order date and naturally remains null when no order matches.
- The final `GROUP BY` establishes one result row per customer.
- Ordering by order count and then customer ID gives stable output.

Verified behavior:

- The endpoint returned all 20 customers without a status filter.
- The four customers without orders had a zero count, zero total, and null last-order date.
- The `pending` and `shipped` filters still returned all 20 customers; 16 had no matching orders for each filter.
- An unknown status returned all 20 customers with zero counts, zero totals, and null dates.

#### Central lesson: know the grain of every query stage

Before choosing an aggregate, identify what one row represents at that point in the query:

- Before joining `order_items`: one row represents one order.
- After joining `order_items`: one row represents one order item belonging to an order.
- After grouping by customer: one row represents one customer.

Unexpected counts and totals often come from losing track of this grain. Decide whether to use a distinct aggregate or to pre-aggregate when moving between grains.

#### Deferred application concerns

Input validation, error responses, cancellation propagation, secret storage, and other cross-cutting API concerns are intentionally deferred while the current focus remains PostgreSQL Chapter 7.

The general syntax is
[With something][select list exptressions] from [table reference,[table reference]]
SELECT a.\* FROM (my_table AS a JOIN your_table AS b ON ...) AS c : a is not available after the parenthesis.

Status:
Table function :
I have understood that table function can be part of from clause
They can be combined like table alias unset(Array[1,2,3]) as table
LATERAL allows a subquery or FROM expression to depend on rows produced by earlier FROM items.
so inside where,select,having inside a subquery, we can access the outer table.

read again
CTE

Data-modifying WITH

1. CTE can contain:
   INSERT / UPDATE / DELETE / MERGE

2. RETURNING creates the CTE's output rows.

3. Other parts should consume RETURNING
   to observe those changes.

4. DML CTEs run exactly once and to completion.

5. Sibling statements share the same snapshot;
   don't assume execution ordering.

6. Never design sibling statements to modify
   the same row.

Data-modifying WITH

1. CTE can contain:
   INSERT / UPDATE / DELETE / MERGE

2. RETURNING creates the CTE's output rows.

3. Other parts should consume RETURNING
   to observe those changes.

4. DML CTEs run exactly once and to completion.

5. Sibling statements share the same snapshot;
   don't assume execution ordering.

6. Never design sibling statements to modify
   the same row.

SELECT
FROM
WHERE
column/table aliases
expressions
DISTINCT
ORDER BY
LIMIT
OFFSET
INNER JOIN
LEFT JOIN
RIGHT JOIN
FULL OUTER JOIN
CROSS JOIN
SELF JOIN
multiple joins
GROUP BY
HAVING
COUNT / SUM / AVG / MIN / MAX
subqueries
correlated subqueries
derived tables
CTEs
UNION
UNION ALL
INTERSECT
EXCEPT
window functions
PARTITION BY
ROW_NUMBER
RANK
DENSE_RANK
pagination basics

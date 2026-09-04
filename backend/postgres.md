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

## Chapter 7 backend mastery exercises

Reference: https://www.postgresql.org/docs/current/queries.html

The exercises below follow Chapter 7, but they are ordered by usefulness for backend work rather than by documentation section. Continue using the existing `customers`, `orders`, and `order_items` practice database unless an exercise explicitly asks for another table.

Priority meanings:

- **Core**: expected in day-to-day backend development. Complete these without hints.
- **Useful**: common enough that a backend developer should recognize and apply it.
- **Advanced**: understand the purpose and complete once; memorizing every detail is unnecessary.

For every API exercise:

1. Write and run the SQL directly before adding it to C#.
2. State what one row represents at every query stage.
3. Select explicit columns; do not use `SELECT *` in the API query.
4. Use parameters for every client-supplied value.
5. Give the final result a deterministic order.
6. Test normal results, no results, nulls, ties, and boundary values.
7. Pass the request cancellation token to every asynchronous Npgsql call.
8. Inspect important queries with `EXPLAIN (ANALYZE, BUFFERS)` after first proving correctness.

### Completed foundation

- [x] **Exercise 1 — Customer order history:** inner joins, filtering, grouping, aggregate order totals, ordering, and `LIMIT`.
- [x] **Exercise 2 — Customer order summary:** left join, derived table, optional status filter, aggregation, `COUNT(DISTINCT ...)`, `COALESCE`, and null handling.

### Phase 1 — Filtering and query semantics

#### Exercise 3 — Build an order search endpoint (Core)

Implement `GET /orders` with optional `status`, `customerId`, `placedFrom`, `placedTo`, `minimumTotal`, and `maximumTotal` filters.

Practice:

- Build optional predicates safely with parameters.
- Use `WHERE` for row-level filters and `HAVING` for filters that depend on an aggregate.
- Treat the time interval as half-open: `placed_at >= from AND placed_at < to`.
- Decide and document what a missing parameter means.
- Return zero rows rather than treating an empty result as an error.

Verify:

- Supplying no filters returns every order.
- Each filter works alone and in combination.
- Orders exactly at the lower bound are included and orders exactly at the upper bound are excluded.
- `minimumTotal` does not accidentally filter individual line items before the order total is calculated.

#### Exercise 4 — Find customers with and without matching orders (Core)

Write two versions of each query below: one using `EXISTS`/`NOT EXISTS`, and one using a join.

- Customers who have at least one shipped order.
- Customers who have never placed an order.
- Customers who placed orders but have never placed a cancelled order.

Practice:

- Correlated subqueries.
- `EXISTS` as a test for the existence of a row, independent of its selected value.
- The difference between `NOT EXISTS` and `NOT IN` when nulls are possible.
- Avoiding duplicated customers from a one-to-many join.

#### Exercise 5 — Prove the `ON` versus `WHERE` outer-join difference (Core)

Starting from the customer summary, apply `status = 'shipped'` first inside the `LEFT JOIN ... ON` condition and then in the outer `WHERE` clause.

Deliver:

- Both queries and their results.
- A written explanation of why one retains customers without shipped orders and the other removes them.
- A third correct version that pre-filters orders in a derived table.

This is a critical backend skill because optional relationship filters frequently turn outer joins into accidental inner joins.

### Phase 2 — Result shape, aliases, and distinct rows

#### Exercise 6 — Design a stable API projection (Core)

Create `GET /orders/{orderId}` returning an order header and calculated fields such as `lineCount`, `itemCount`, and `totalAmount`.

Practice:

- Explicit select-list items.
- Expressions and API-friendly column labels.
- `CASE`, `COALESCE`, and type casts in the select list.
- The visibility of input column names versus output aliases.
- Mapping nullable and non-nullable result columns correctly in C#.

Verify that changing the physical column order in a table would not change the API response mapping.

#### Exercise 7 — Latest order per customer (Core)

Return at most one latest order for every customer, including customers with no orders.

Implement and compare:

1. `DISTINCT ON (customer_id)` with a matching `ORDER BY`.
2. `ROW_NUMBER() OVER (PARTITION BY customer_id ORDER BY placed_at DESC, id DESC)`.
3. `LEFT JOIN LATERAL (...) ORDER BY ... LIMIT 1`.

Verify:

- Ties in `placed_at` always pick the same order because `id` is the tiebreaker.
- Customers without orders are retained.
- Explain which version you find most readable and inspect their execution plans.

### Phase 3 — Aggregation and reporting

#### Exercise 8 — Operational order dashboard (Core)

Implement `GET /reports/orders-by-status` for a supplied date range. Return one row per status containing order count, distinct customer count, item count, revenue, average order value, and latest order time.

Practice:

- `GROUP BY` and aggregate functions.
- Conditional aggregation with `FILTER (WHERE ...)`.
- Avoiding incorrect counts after a one-to-many join.
- `HAVING` to retain only groups meeting an aggregate threshold.
- Defining a precise result grain before writing the query.

#### Exercise 9 — Multi-level sales report (Useful)

Produce totals at all of these levels in one query:

- `(order_date, status)`
- `(order_date)`
- `(status)`
- Grand total

Use `GROUPING SETS`, then reproduce a suitable version with `ROLLUP`. Use `GROUPING(...)` to distinguish a subtotal null from a real null value.

Backend lesson: these features are valuable for reporting endpoints, but ordinary CRUD APIs rarely need them. Understand them; do not force them into simple queries.

### Phase 4 — Window functions

#### Exercise 10 — Customer spending leaderboard (Core)

Implement `GET /reports/customer-leaderboard` returning each customer's spending, rank, dense rank, row number, and percentage of total revenue.

Practice:

- Aggregate first, then apply window functions to the aggregate result.
- Compare `ROW_NUMBER`, `RANK`, and `DENSE_RANK` when customers tie.
- Use an empty `OVER ()` window for a grand total.
- Keep final response ordering separate from window ordering.

#### Exercise 11 — Running totals and previous-order comparison (Core)

For one customer, return each order with:

- Its total.
- The previous order time and previous order total using `LAG`.
- The change from the previous total.
- A running lifetime spend using `SUM(...) OVER (...)`.

Specify the window frame explicitly and explain why the default frame can surprise you when ordering values contain ties.

#### Exercise 12 — Top three orders per customer (Core)

Return each customer's three highest-value orders. Include ties in one version and force exactly three rows per customer in another.

Practice:

- Ranking inside a subquery or CTE.
- Why a window-function result cannot normally be filtered in the same query level where it is computed.
- Choosing between `ROW_NUMBER`, `RANK`, and `DENSE_RANK` from product requirements.

### Phase 5 — Combining result sets

#### Exercise 13 — Unified customer activity feed (Useful)

Add a small `customer_notes` table or another customer event source. Build a feed that combines orders and notes into the same result shape using `UNION ALL`.

Each feed row should contain `customer_id`, `event_id`, `event_type`, `occurred_at`, and `description`.

Practice:

- Matching column counts and compatible data types across queries.
- Adding a discriminator such as `event_type`.
- Applying one final `ORDER BY` to the combined result.
- Understanding why `UNION ALL` is normally preferable when deduplication is not required.

#### Exercise 14 — Compare customer sets (Useful)

Use set operations to find:

- Customers who ordered in either of two date ranges (`UNION`).
- Customers who ordered in both ranges (`INTERSECT`).
- Customers who ordered in the first range but not the second (`EXCEPT`).

Repeat one query with `UNION ALL` and explain duplicate handling. Add parentheses when a branch needs its own `ORDER BY` or `LIMIT`.

### Phase 6 — Sorting and production pagination

#### Exercise 15 — Multi-column dynamic sorting (Core)

Extend an order-list endpoint to support a small allowlist of sort modes: newest, oldest, highest value, and status then newest.

Practice:

- Multiple sort keys and mixed ascending/descending directions.
- `NULLS FIRST` and `NULLS LAST`.
- Always appending a unique tiebreaker such as `id`.
- Mapping an API enum to predefined SQL fragments; never place unchecked user input into `ORDER BY`.

#### Exercise 16 — Offset pagination and its limits (Core)

Implement page-number pagination using `LIMIT` and `OFFSET`.

Verify:

- Every page has a deterministic order.
- Page boundaries contain no duplicates when the data is unchanged.
- Insert an order between fetching page 1 and page 2 and observe whether rows shift.
- Compare `EXPLAIN (ANALYZE, BUFFERS)` for offsets 0, 100, 10,000, and 100,000 after generating enough test data.

Write down why PostgreSQL still has to compute skipped rows and when offset pagination remains acceptable.

#### Exercise 17 — Keyset/cursor pagination (Core)

Replace the previous exercise with cursor pagination ordered by `(placed_at DESC, id DESC)`.

API inputs:

- `pageSize`
- Optional `beforePlacedAt`
- Optional `beforeOrderId`

Practice:

- Composite comparison: `(placed_at, id) < (@beforePlacedAt, @beforeOrderId)`.
- Matching the comparison direction to the sort direction.
- Fetching `pageSize + 1` rows to calculate `hasNextPage`.
- Returning an opaque next cursor to the client.
- Adding and validating an index beginning with the equality-filter columns and followed by the ordering columns.

Verify page traversal while concurrent inserts occur. Document the tradeoff: keyset pagination supports next/previous traversal efficiently but does not naturally jump to an arbitrary page number.

### Phase 7 — `VALUES`, table functions, and `LATERAL`

#### Exercise 18 — Join request data with database data using `VALUES` (Useful)

Given a small batch of requested order IDs and client correlation IDs, represent them as a typed `VALUES` table and left join it to `orders`.

Return one row for every requested ID, including IDs that do not exist.

Practice:

- Treating constants as a table.
- Giving the table and its columns aliases.
- Explicit casts when PostgreSQL cannot infer a parameter type.
- Preserving input-to-output correlation in a small batch request.

For large or variable batches, also learn the practical alternatives: array parameters with `unnest`, JSON recordsets, temporary tables, or bulk loading.

#### Exercise 19 — Expand arrays with ordinality (Useful)

Pass an ordered array of order IDs, expand it with `unnest(... ) WITH ORDINALITY`, join to `orders`, and return results in the same order supplied by the caller.

Verify duplicates and missing IDs. Decide whether the endpoint should retain or remove duplicate requested IDs.

#### Exercise 20 — Top-N child rows with `LATERAL` (Useful)

For every customer, return their latest two orders using `LEFT JOIN LATERAL`. Retain customers with no orders.

Then solve the same problem with a window function and compare:

- Readability.
- Whether the query returns exactly the required grain.
- The execution plan with an index on `(customer_id, placed_at DESC, id DESC)`.

### Phase 8 — CTEs and query composition

#### Exercise 21 — Refactor a complex report with CTEs (Core)

Rewrite the customer leaderboard as named stages:

1. Compute one row per order.
2. Compute one row per customer.
3. Rank customers.
4. Select the API response shape.

Practice:

- Using CTEs to make grain changes explicit.
- Naming CTEs after the result they contain rather than the operation performed.
- Comparing the CTE version to a derived-table version for correctness and readability.

Do not assume a CTE is automatically faster or that it is always materialized.

#### Exercise 22 — Observe CTE materialization (Advanced)

Create a CTE referenced once and another referenced twice. Compare default behavior with `MATERIALIZED` and `NOT MATERIALIZED` using `EXPLAIN (ANALYZE, BUFFERS)`.

Record:

- Whether predicates were pushed into the underlying scan.
- Whether expensive work was repeated.
- Why forcing either behavior without measuring can hurt performance.

The backend goal is recognizing a performance issue during query review, not routinely adding these keywords.

#### Exercise 23 — Archive rows with a data-modifying CTE (Useful, use a transaction)

Create an `archived_orders` table in the practice database. In one statement, delete eligible old orders, capture them with `RETURNING`, and insert the returned rows into the archive table.

Practice:

- A data-modifying statement inside `WITH`.
- Consuming the `RETURNING` rows rather than expecting the modified table itself to be the CTE result.
- Verifying affected-row counts and rollback behavior.
- Understanding that sibling data-modifying CTEs share a snapshot and must not be designed to modify the same row.

Run this only against disposable practice data and wrap initial attempts in `BEGIN`/`ROLLBACK`.

### Phase 9 — Recursive queries

#### Exercise 24 — Category hierarchy API (Useful)

Add a `categories(id, parent_id, name)` table and implement:

- All descendants of a category.
- The ancestor path from a category to the root.
- A displayed depth for each result.
- Stable depth-first or breadth-first output using the documented `SEARCH` syntax or an explicit ordering column.

Practice the non-recursive seed term, `UNION ALL`, recursive term, and termination condition.

#### Exercise 25 — Prevent cycles in hierarchical data (Advanced)

Create deliberately cyclic practice data and make the recursive query safe using path tracking or the `CYCLE` clause.

Verify:

- The query terminates.
- Cyclic rows are identifiable.
- The returned path is useful for diagnosis.

Backend lesson: a recursive query must not rely on the data being perfect unless the schema independently guarantees that cycles are impossible.

### Final integration exercise

#### Exercise 26 — Production-style order browsing endpoint (Core capstone)

Build one endpoint that combines the most important Chapter 7 skills:

- Optional parameterized filters.
- Correct joins and aggregation.
- A derived table or CTE with an explicitly documented grain.
- A calculated order total.
- Deterministic allowlisted sorting.
- Keyset pagination and a next cursor.
- A stable explicit response projection.
- Cancellation propagation.

Acceptance checks:

- No SQL text is constructed from unchecked client values.
- Empty, null, tied, and boundary cases are tested.
- No duplicate or skipped rows occur while traversing unchanged data.
- The main access path is supported by an appropriate index.
- `EXPLAIN (ANALYZE, BUFFERS)` is saved and explained in plain language.
- The endpoint's result shape and pagination contract are documented.

### Recommended completion order

Complete the exercises in this order:

`3 → 4 → 5 → 6 → 7 → 8 → 10 → 11 → 12 → 15 → 16 → 17 → 21 → 26`

That sequence covers the Chapter 7 knowledge most frequently needed in backend work. Then complete exercises `9, 13, 14, 18, 19, 20, 22, 23, 24, 25` for breadth.

Chapter 7 is mastered for backend purposes when you can look at a requirement, define the result grain, write a correct parameterized query, make its ordering stable, choose pagination deliberately, and explain the important parts of its execution plan. Memorizing uncommon syntax is not the goal.

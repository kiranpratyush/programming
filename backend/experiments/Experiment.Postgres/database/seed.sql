begin;
INSERT into customers(name,email,created_at)
SELECT
    'Customer ' || customer_number,
    'customer' || customer_number || '@example.com',
    timestamptz '2025-12-01 10:00:00+05:30'
        + customer_number * interval '1 day'
FROM generate_series(1, 20) AS customer_number;

INSERT INTO orders
(
    customer_id,
    placed_at,
    status,
    currency
)
SELECT
    ((order_number - 1) % 16) + 1,
    timestamptz '2026-01-01 09:00:00+05:30'
        + order_number * interval '12 hours',
    CASE order_number % 4
        WHEN 0 THEN 'cancelled'
        WHEN 1 THEN 'pending'
        WHEN 2 THEN 'paid'
        ELSE 'shipped'
    END,
    'INR'
FROM generate_series(1, 100) AS order_number;

INSERT INTO order_items
(
    order_id,
    product_name,
    quantity,
    unit_price
)
SELECT
    o.id,
    'Product ' || (((o.id + item_number - 1) % 12) + 1),
    1 + ((o.id + item_number) % 4)::integer,
    (
        100
        + ((o.id * item_number * 37) % 900)
    )::numeric(12, 2)
FROM orders AS o
CROSS JOIN generate_series(1, 2) AS item_number;

COMMIT;
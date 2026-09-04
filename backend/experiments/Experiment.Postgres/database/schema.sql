begin;
CREATE TABLE customers (
    id bigint generated always as identity primary key,
    name text not null,
    email text not null unique,
    created_at  timestamptz not null default now(),
    CONSTRAINT customers_name_not_blank
        CHECK (length(trim(name)) > 0),

    CONSTRAINT customers_email_not_blank
        CHECK (length(trim(email)) > 0)
);
CREATE TABLE orders
(
    id          bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    customer_id bigint      NOT NULL,
    placed_at   timestamptz NOT NULL DEFAULT now(),
    status      text        NOT NULL,
    currency    varchar(3)  NOT NULL DEFAULT 'INR',

    CONSTRAINT orders_customer_fk
        FOREIGN KEY (customer_id)
        REFERENCES customers (id),

    CONSTRAINT orders_status_valid
        CHECK (status IN ('pending', 'paid', 'shipped', 'cancelled')),

    CONSTRAINT orders_currency_length
        CHECK (length(currency) = 3)
);
CREATE TABLE order_items
(
    id           bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    order_id     bigint         NOT NULL,
    product_name text           NOT NULL,
    quantity     integer        NOT NULL,
    unit_price   numeric(12, 2) NOT NULL,

    CONSTRAINT order_items_order_fk
        FOREIGN KEY (order_id)
        REFERENCES orders (id)
        ON DELETE CASCADE,

    CONSTRAINT order_items_product_name_not_blank
        CHECK (length(trim(product_name)) > 0),

    CONSTRAINT order_items_quantity_positive
        CHECK (quantity > 0),

    CONSTRAINT order_items_unit_price_non_negative
        CHECK (unit_price >= 0)
);

CREATE INDEX ix_orders_customer_placed_at
    ON orders (customer_id, placed_at DESC, id DESC);

CREATE INDEX ix_order_items_order_id
    ON order_items (order_id);

commit;

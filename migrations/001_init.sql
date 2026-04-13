PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS schema_migrations (
    version INTEGER PRIMARY KEY,
    applied_ts TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ', 'now'))
); 

CREATE TABLE IF NOT EXISTS products (
    id INTEGER PRIMARY KEY,
    sku TEXT NOT NULL UNIQUE,
    name TEXT NOT NULL,
    unit TEXT NOT NULL DEFAULT 'each'
);

CREATE TABLE IF NOT EXISTS locations (
    id INTEGER PRIMARY KEY,
    code TEXT NOT NULL UNIQUE,
    name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS stock_event (
    id INTEGER PRIMARY KEY,
    ts TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ', 'now')),
    type TEXT NOT NULL,
    product_id INTEGER NOT NULL REFERENCES products(id),
    from_location_id INTEGER REFERENCES locations(id),
    to_location_id INTEGER REFERENCES locations(id),
    qty_delta REAL NOT NULL,
    reference TEXT,
    note TEXT,
    user TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS stock_snapshot (
    product_id INTEGER NOT NULL REFERENCES products(id),
    location_id INTEGER NOT NULL REFERENCES locations(id),
    qty_on_hand REAL NOT NULL DEFAULT 0,
    qty_reserved REAL NOT NULL DEFAULT 0,
    PRIMARY KEY(product_id, location_id)
);

CREATE INDEX IF NOT EXISTS idx_events_products_ts ON stock_event(product_id, ts);

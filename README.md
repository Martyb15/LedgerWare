# LedgerWare
A command-line inventory ledger built with C++ 20 and SQLite. Record stock movements, query current inventory, and maintain. 

## What it does
LedgerWare tracks inventory the way a warehouse system would; every stock change is recorded as an immutable event, and current quantities are derived from those events. You receive stock, query balances, and get a clear history of what changed, when, and by whom. 


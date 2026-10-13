## Valid Strings

| Input | Reason |
|---|---|
| `id` | Single identifier |
| `id+id` | Addition |
| `id*id` | Multiplication |
| `id+id*id` | Addition and multiplication |
| `(id)` | Parenthesized expression |
| `(id+id)` | Addition inside parentheses |
| `id*id+id` | Multiplication followed by addition |
| `(id+id)*id` | Parenthesized addition followed by multiplication |
| `id+id+id` | Multiple additions |
| `id*id*id` | Multiple multiplications |

## Invalid Strings

| Input | Reason |
|---|---|
| `+id` | Starts with an operator |
| `id+` | Missing operand after `+` |
| `id*` | Missing operand after `*` |
| `id++id` | Consecutive addition operators |
| `id**id` | Consecutive multiplication operators |
| `(id+id` | Missing closing parenthesis |
| `id+id)` | Unmatched closing parenthesis |
| `()` | Empty parentheses |
| `id(id)` | Missing operator before parentheses |
| `*id` | Starts with multiplication |
| `id+*id` | Missing operand between operators |

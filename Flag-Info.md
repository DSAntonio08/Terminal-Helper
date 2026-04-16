# 🛠️ Terminal Helper - Flag Reference

This tool helps you navigate your bash history efficiently. Use the flags below to filter and limit your search results.

## Available Flags

| Flag | Name   | Description                                                      | Requirement                  |
|------| -------| -----------------------------------------------------------------|------------------------------|
| `-s` | **Search** | Triggers the search engine to look through your `.bash_history`. | Requires a **keyword** argument. |
| `-m` |**Matches** | Sets a limit on how many search results are displayed.           | Requires a **numeric** value.    |

---

## 💡 Usage Examples

### 1. Basic Search
Search for every time you used `gcc`:
```bash
./terminal-helper -s gcc
```

### 2. Limited Search
Search for the last 5 times you used `ls`:
```bash
./terminal-helper -s -m 5 ls
```

### 3. Combined Flags (Short-hand)
You can combine the flags, but remember that the keyword must come last:
```bash
./terminal-helper -sm 10 ssh
```

---

## Important Notes
* **Flag Order:** If you use `-m`, it must be followed immediately by a number.
* **Default Limit:** If `-m` is not provided, the helper defaults to finding **128** matches.
* **Keyword:** The search keyword should always be the final argument in your command.


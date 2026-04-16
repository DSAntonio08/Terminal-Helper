🛠️ Terminal Helper - Flag Reference

This tool searches your `~/.bash_history` file for previously used commands.

## Available Flags

| Flag   | Long Name   | Description                                                      | Requirement                               |
|--------|-------------|------------------------------------------------------------------|-------------------------------------------|
| `-s`     | **Search**      | Enables searching through your `.bash_history` file                | Optional (automatically enabled with `-m`)  |
| `-m`     | **Matches**     | Sets the maximum number of results to display                    | Required: a number (e.g. `-m10` or `-sm10`)   |
| `-h`     | **Help**        | Displays this help message                                       |                                           |

## Usage Examples

### 1. Basic Search
```bash
./terminal-helper -s gcc
```

###  2. Search with Limit
Recommended ways:
```bash
Bash./terminal-helper -sm10 gcc          # Short form (recommended)
./terminal-helper -s -m 10 gcc       # Clear and explicit
./terminal-helper -m 10 -s gcc
```

### 3. Lazy Mode (without -s)
Since -m automatically enables search, you can skip the -s flag:
```bash
./terminal-helper -m 20 python
./terminal-helper -m 1 gcc
```

### 4. Show Help
```bash
./terminal-helper -h
```

## Important Notes

The **order** of arguments **matters**:
The search keyword (e.g. gcc, ls, cd) must always be the **last** argument.
When using combined flags, write -sm10 or -sm 10. 

Default limit is 128 matches if -m is not used.



# Tips
## Listening ports on device
Run `netstat -tuln` to see what ports are listening on your device when you want to troubleshoot.

# Coding format
Use the coding format specified in the .clang-format file

## Automatic formating with ...

### Zed
Add this to your settings
```
  "languages": {
    "C": {
      "format_on_save": "on"
    }
  },
  "file_types": {
    "C": ["c", "h"]
  }
```

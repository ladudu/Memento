# Memento Translations

This directory contains translation files for the Memento user interface.

## Translation Workflow

### 1. Extract translatable strings
```bash
lupdate src/ -ts translations/memento_zh_CN.ts
```

### 2. Translate using Qt Linguist
```bash
linguist translations/memento_zh_CN.ts
```

### 3. Compile translations to binary format
```bash
lrelease translations/memento_zh_CN.ts
```

## Supported Languages

- English (default)
- Chinese Simplified (zh_CN) - In progress

## Notes

- Only the user interface is translated
- Dictionary content and learning features remain Japanese-only
- This is a Japanese learning tool

# TeleCloud

![Toolkit: QT](https://img.shields.io/badge/Toolkit-QT-informational?color=41CD52&logo=qt&logoColor=white)  
![Language: C++ / QT](https://img.shields.io/badge/language-C%2B%2B%20%2F%20QT-orange)  

TeleCloud is a Qt/C++ app that uses a Telegram bot + channel to store unlimited stream recordings in Telegram.  
It works in tandem with [ctbrec](https://github.com/0xboobface/ctbrec) and a local [tdlight-telegram-bot-api](https://github.com/tdlight-team/tdlight-telegram-bot-api).

---

## 📸 Screenshot

<img width="323" height="166" alt="image" src="https://github.com/user-attachments/assets/7eadae61-089b-4821-9d06-82e7b1ba4e96" />

---

## 📦 Build Requirements

- Qt **5.12+**  
- C++17 compiler  

## 🔧 Runtime / Use Requirements

1. `ctbrec` **5.3.3+**  
2. Local Telegram API (`tdlight-telegram-bot-api`)  
3. Telegram Bot  
4. Telegram Channel  

---

## ⚙️ Setup Before Building

1. Copy `config/auth.example.ini` → `config/auth.ini`  
   - Replace `BOT_API_TOKEN` with your bot token  
   - Replace `CHANNEL_ID` with your channel ID  

2. Copy `config/config.example.ini` → `config/config.ini`  
   - Set `RECORDINGS_JSON_PATH` to the absolute path where ctbrec outputs .json files  
   - Optionally adjust `SQLITE_DB_PATH`  

3. During build, config files will be auto-copied next to the executable.

---

## ▶️ How to Use

1. Launch **local Telegram API** (keep default port)  
2. Run **TeleCloud**  
3. Press **Sync Metadata** — reads ctbrec `.json` files and finds new recordings  
4. Press **Fill Queue** — enqueues new files  
5. Press **Start** — begins uploading  
6. Press **Stop** — stops after finishing the current file  

If *“Create contact sheet”* is enabled in ctbrec, TeleCloud auto-detects and uploads them as reply messages to their video.

---

## ✅ Database

The app will automatically create the SQLite database (and tables) if it doesn't already exist, you can find the links to all uploaded posts in **uploads** table.  

---

## 📝 License & Badges

This project is available under the **MIT License**.  

---

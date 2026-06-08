# 🔍 FlashSearch: High-Performance Incremental Search Engine (C++)

A high-speed desktop search utility built in C++ that provides ranked search results using **TF-IDF relevance scoring**. Unlike standard search scripts, FlashSearch features an **Incremental Indexing Engine** that tracks file state to ensure near-instant updates.

---

## ✨ Features

- **Ranked Retrieval:** Implements log-normalized **TF-IDF** (Term Frequency-Inverse Document Frequency) to ensure the most relevant documents appear at the top.
- **Incremental Indexing:** Uses `std::filesystem` metadata to track "Last Modified" timestamps. It only re-indexes files that have been added or changed, ignoring thousands of untouched files to save CPU cycles.
- **Multithreaded Indexing:** Utilizes hardware-concurrency-aware batch processing to index large sets of documents in parallel using `std::thread` and `std::mutex`.
- **Fuzzy Matching:** Built-in "Did you mean?" functionality using a space-optimized **Levenshtein Distance (Edit Distance)** algorithm to handle user typos.
- **Auto-Discovery & Deletion Tracking:** Automatically crawls the `/data` directory and cleans up "stale" entries from the index if a file is deleted from the disk.
- **Contextual Snippets:** Provides a "Google-style" preview, extracting and displaying the exact sentence where the match was found.
- **Dual-Layer Persistence:** Serializes both the Inverted Index and File Metadata to disk, allowing the system to resume state instantly upon restart.

---

## 🛠️ System Architecture

### 1. Preprocessing Pipeline

- **Normalization:** Case-folding (lowercase) and punctuation stripping using `isalnum`.
- **Filtering:** Stop-word removal (e.g., "the", "is", "and") to reduce noise in the inverted index.

### 2. Data Structures

- **Primary Index:** `unordered_map<string, unordered_map<string, int>>` (Inverted Index) for $O(1)$ word lookup across the dataset.
- **State Registry:** `unordered_map<string, long long>` storing epoch-based timestamps for change detection and cache invalidation.

### 3. Ranking Logic

The engine uses log-normalized TF-IDF for scoring:
$$\text{Score} = (1.0 + \log_{10}(\text{TF})) \times \log_{10}(1.0 + \frac{\text{Total Docs}}{\text{Docs With Word}})$$

---

## 🔧 Tech Stack

- **Language:** C++ (Standard 17+)
- **Libraries:** STL (`unordered_map`, `vector`, `set`, `filesystem`, `thread`, `mutex`)
- **Build System:** CMake
- **I/O:** `fstream` (State Serialization)

---

## 💻 Usage & Example

**Input:**

Enter Query: data structures
Output:

[1] data/ds_notes.txt (Rank: 3.45)
    Snippet: ... the fundamental concepts of data structures and algorithms ...

[2] data/assignment_1.txt (Rank: 1.23)
    Snippet: ... please submit the data structures project by Friday ...


## 📂 Project Structure

<pre>
```text
FlashSearch/
├── src/
│ ├── main.cpp # Application Entry Point
│ ├── SearchEngine.cpp # Logic Implementation
│ └── SearchEngine.h # Class Definitions
├── data/ # Target directory for .txt files
├── CMakeLists.txt # Build Configuration
└── README.md # Documentation
```
</pre>


## 🚀 Installation & Getting Started

Prerequisites
C++ Compiler: GCC (v9+) or Clang.
Build System: CMake (v3.10+).
Library: pthread (required for multithreading).

1. Build the Project

# Navigate to the project folder
cd FlashSearch

# Create a build directory
mkdir build

cd build

# Generate build files and compile
cmake ..

make

2. Run the Engine

# Ensure you have a /data folder with .txt files inside the root or build folder

./FlashSearch


## 🎓 Learning Outcomes

Developed a deep understanding of Inverted Indexing and Information Retrieval.
Mastered Concurrency in C++ using Mutexes and Thread Pools.
Implemented Object-Oriented Design to separate indexing logic from the user interface.
Solved the Cache Invalidation problem using metadata tracking.
Applied Dynamic Programming for space-optimized fuzzy string matching.


# 👤 Author
Shivam Kumar
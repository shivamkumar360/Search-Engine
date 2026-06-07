#   FlashSearch: High-Performance Incremental Search Engine (C++)

A high-speed desktop search utility built in C++ that provides ranked search results using TF-IDF relevance scoring. Unlike standard search scripts, FlashSearch features an Incremental Indexing Engine that tracks file state to ensure near-instant updates.

---

# Features


-  Ranked Retrieval: Implements log-normalized TF-IDF (Term Frequency-Inverse Document Frequency) to ensure the most relevant 
   documents appear at the top.

-  Incremental Indexing: Uses std::filesystem metadata to track "Last Modified" timestamps. It only re-indexes files that have 
   been added or changed, ignoring thousands of untouched files to save CPU cycles.  

-  Stop-word removal (e.g., "the", "is")  

-  Auto-Discovery & Deletion Tracking: Automatically crawls the /data directory and cleans up "stale" entries from the index if a 
   file is deleted from the disk.  

-  Contextual Snippets: Provides a "Google-style" preview of the search results, extracting and displaying the exact sentence
   where the match was found.

-  Dual-Layer Persistence: Serializes both the Inverted Index and File Metadata to disk, allowing the system to resume state 
   instantly upon restart.


---

# How it Works

1. Preprocessing
   - Convert text to lowercase  
   - Remove punctuation  
   - Remove stop words  

2. Indexing
   - Build an inverted index  
   - Store frequency of each word per file  

3. Persistence
   - The index is written to disk in a structured format (word file1:freq file2:freq) for rapid loading

4. Ranking
   - Use TF-IDF:
     
     score = (1.00 + log10(frequency)) × log(1 + total_docs / docs_with_word)
     

5. Query
   - Input query → tokenize → compute scores → rank results  



# Example

# Input: data structures


# Output:
data.txt -> 3.45
data2.txt -> 1.23




# Tech Stack

- C++
- STL (unordered_map, vector, set)
- File Handling (ifstream)

1. Preprocessing Pipeline

Normalization: Case-folding (lowercase) and punctuation stripping using isalnum.
Stop-word Filtering: High-frequency, low-info words (e.g., "the", "is", "and") are filtered out to improve ranking accuracy.

2. Data Structures

Primary Index: unordered_map<string, unordered_map<string, int>>

State Registry: unordered_map<string, long long> storing epoch-based timestamps for change detection.

3. Ranking Logic
The engine uses the following formula for document scoring:

Score = (1.0 + log10(TF)) * log10(1.0 + Total_Docs / Docs_With_Word);


# Usage and Example

Input: 
Enter Query: data structures

Output:

[1] data/notes.txt (Rank: 3.45)
    Snippet: ... the fundamental concepts of data structures and algorithms ...

[2] data/assignment_1.txt (Rank: 1.23)
    Snippet: ... please submit the data structures project by Friday ...






---

# Learning Outcomes



- Developed a deep understanding of **Inverted Indexing** and Information Retrieval.

- Implemented Object-Oriented Design to separate indexing logic from the search interface.

- Solved the Cache Invalidation problem using **metadata tracking**.

- Applied hashing for efficient search  


# Future Roadmap

- **Multi-threading**: Parallelize the initial indexing phase using std::thread and std::mutex.

- **Stemming**: Integrate a Porter Stemmer to handle word variations (e.g., "searching" -> "search").

- **Fuzzy Matching**: Implement Levenshtein Distance for typo tolerance.


# Author

Shivam Kumar












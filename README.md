#   Text Search Engine (C++)

A ligthweight search engine system built in C++ that performs automated document discovery, inverted indexing, and relevance ranking using Log-Normalized TF-IDF.

---

# Features

-  Inverted Index for fast lookup  
-  TF-IDF (log-scaled) ranking  
-  Stop-word removal (e.g., "the", "is")  
-  Query-based document retrieval  
-  Ranked results (most relevant first)  
-  Efficient using unordered_map (hashing)
-  Dynamic File Discovery: Automatically crawls a local directory (/data) and indexes all .txt files using the C++17 
   (using std::filesystem)
-  Persistence Layer: Serializes the inverted index to index.txt to avoid redundant re-processing, significantly reducing startup time.

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

---

# Learning Outcomes

- Implemented inverted indexing  
- Understood TF-IDF ranking  
- Applied hashing for efficient search  
- Built a mini information retrieval system  

# Future Roadmap

- Stemming: Integrate Porter Stemmer to handle word variations (e.g., "running" -> "run").
- 
- Multi-threading: Parallelize the indexing of large file batches.
- 
- Boolean Logic: Support for AND, OR, and NOT operators.
- 
- Fuzzy Search: Implement Levenshtein Distance for typo tolerance.



# Author

Shivam Kumar
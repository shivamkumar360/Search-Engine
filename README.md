#  Search Engine in C++

A lightweight search engine built in C++ that indexes text documents and ranks them using TF-IDF.

---

# Features

-  Inverted Index for fast lookup  
-  TF-IDF (log-scaled) ranking  
-  Stop-word removal (e.g., "the", "is")  
-  Query-based document retrieval  
-  Ranked results (most relevant first)  
-  Efficient using unordered_map (hashing)

---

# How it Works

1. Preprocessing
   - Convert text to lowercase  
   - Remove punctuation  
   - Remove stop words  

2. Indexing
   - Build an inverted index  
   - Store frequency of each word per file  

3. Ranking
   - Use TF-IDF:
     
     score = frequency × log(1 + total_docs / docs_with_word)
     

4. Query
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



# Author

Shivam Kumar